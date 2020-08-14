import time
import zmq

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://127.0.0.1:5555")
socket.setsockopt(zmq.SUBSCRIBE, b'')
# while True:
for i in range(100):
    message = socket.recv()
    print(message)
    # time.sleep(1)
    # socket.send_string("server response!")
