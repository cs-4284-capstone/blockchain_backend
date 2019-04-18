import subprocess

from flask import Flask
app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello World!"

@app.route("/start")
def start():
    subprocess.call(['/contracts/eos_init.sh'], timeout=60)
    return 'OK'

@app.route("/songs/add/<int:songid>/<int:price>")
def songadd(songid, price):
    subprocess.call(["sh", "-c", "cat /contracts/soundbin_wallet.txt | cleos wallet unlock -n soundbin"])
    subprocess.call(['cleos', 'push', 'action', 'music', 'addsong', '[%d, %d]' % (songid, price), '-p', 'music@active'], timeout=120)
    return 'OK'

app.run(host='0.0.0.0')