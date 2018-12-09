from model import prepare_model
from main import mfcc_size, window_size, MAX_MINDWAVE_VALUE, MAX_RAW
import socket
import struct
import librosa
import numpy as np

model = prepare_model(window_size, mfcc_size)
model.load_weights("model6.hdf5")

message_size = 520


def run_server(port=4242):
    host = ''
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        print("waiting")
        s.bind((host, port))
        s.listen(1)
        conn, addr = s.accept()
        fft = [0.] * window_size * 8
        raw = [0.] * window_size * 512

        while(True):
            msg = conn.recv(4 * message_size)
            received = struct.unpack('<%si' % message_size, msg)
            normalized_fft = [x / MAX_MINDWAVE_VALUE for x in received[512:]]
            assert len(normalized_fft) == 8
            normalized_raw = [x / MAX_RAW for x in received[:512]]
            fft = fft[8:] + normalized_fft
            raw = raw[512:] + normalized_raw
            mfcc = librosa.feature.mfcc(np.array(raw), 512, n_mfcc=mfcc_size)

            prediicted = model.predict([np.array([fft]), np.expand_dims(np.array([raw]), axis=2), np.expand_dims(np.array([mfcc]), axis=3)])
            print(prediicted)
            res = struct.pack('<fff', prediicted[0][0], prediicted[0][1], prediicted[0][2])
            conn.sendall(res)


if __name__ == '__main__':
    run_server()