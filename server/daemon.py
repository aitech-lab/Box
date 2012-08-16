import bottle
from bottle import route, post, run, request

bottle.debug(True)

@route('/')
def home():
    try:
        return "Hello Arduino!"
    except Exception, e:
        print e

run(host='193.232.102.70', port=8515, reloader=True)
