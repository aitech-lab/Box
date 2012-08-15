import bottle
from bottle import route, post, run, request

bottle.debug(True)

@route('/')
def home():
    try:
        return "Helo Shurik!"
    except Exception, e:
        print e

run(host='10.0.0.9', port=8515, reloader=True)
