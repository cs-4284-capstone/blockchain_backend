import subprocess

from flask import Flask
app = Flask(__name__)

class WalletContext:
    def __init__(self, pass_file: str, wallet: str):
        self.pass_file = pass_file
        self.wallet = wallet

    def __enter__(self):
        print("Unlocking wallet...")
        subprocess.run(["sh", "-c", f"cat {self.pass_file} | cleos wallet unlock -n {self.wallet}"])
        return True

    def __exit__(self, type, value, traceback):
        print("Locking wallet...")
        subprocess.run(["sh", "-c", f"cleos wallet lock -n {self.wallet}"])


@app.route("/")
def hello():
    return """
    Endpoints:
    - /start: runs eos_init.sh.
    - /songs/add/<int:songid>/<int:price>: adds a songid to the contract at the listed price.
    """

@app.route("/start", methods=['POST'])
def start():
    subprocess.run(['/contracts/eos_init.sh'], timeout=60, check=True)
    return 'OK'

@app.route("/songs/add/<int:songid>/<float:price>", methods=['POST'])
def songadd(songid, price):
    intprice = int(price) # TODO make contract support floats

    with WalletContext("/contracts/soundbin_wallet.txt", "soundbin"):
        subprocess.run(['cleos', 'push', 'action', 'music', 'addsong', '[%d, %d]' % (songid, price), '-p', 'music@active'], timeout=120, check=True)
    
    return 'OK'

app.run(host='0.0.0.0')