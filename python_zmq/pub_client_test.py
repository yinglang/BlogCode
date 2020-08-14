import zmq
import time

context = zmq.Context()
recv_socket = context.socket(zmq.SUB)
send_socket = context.socket(zmq.PUB)
recv_socket.connect("tcp://39.106.11.90:6002")
send_socket.connect("tcp://39.106.11.90:6001")

topic = "m_topic_data"
recv_socket.subscribe(topic)
while True:
    message = recv_socket.recv_multipart()
    print("recv:", message)
    
    # deal_msg_here(message)
    
    ret_str = "hello world"
    send_socket.send_string(topic, zmq.SNDMORE)
    send_socket.send_string(ret_str)
    print("send:", ret_str)
    
    time.sleep(1)
