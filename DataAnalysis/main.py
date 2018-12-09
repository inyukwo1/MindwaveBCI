import glob
import numpy as np
from keras.callbacks import ModelCheckpoint
from keras.models import Sequential, Model
from keras.layers import Dense, Activation, Dropout, BatchNormalization, Input, Conv1D, Flatten, concatenate, add
from keras.metrics import categorical_accuracy
from random import shuffle
import librosa
from model import prepare_model

MAX_MINDWAVE_VALUE = 5000000 # do we have to clip??
MAX_RAW = 2047

window_size = 3
mfcc_size = 20

def load_data():
    datas = [[], [], []]
    raws = [[], [], []]
    mfccs = [[], [], []]
    for file_name in glob.glob("data/12_10_right/train2/*.txt"):
        with open(file_name) as f:
            temp_9_fft = []
            temp_9_raw = []
            for linenum, line in enumerate(f):
                if linenum % 10 == 0:
                    continue
                data_line = [int(i) for i in line.split()]
                if len(data_line) != 521:
                    print(file_name)
                    print(linenum)
                    print(len(data_line))
                assert len(data_line) == 521
                normalized_fft = [x / MAX_MINDWAVE_VALUE for x in data_line[512:520]]
                normalized = [x / MAX_RAW for x in data_line[:512]]
                temp_9_fft.append(normalized_fft)
                temp_9_raw.append(normalized)
                if linenum % 10 == 9:
                    for i in range(9 - window_size + 1):
                        concatenated = []
                        concatenated_raw = []
                        for j in range(window_size):
                            concatenated += temp_9_fft[i + j]
                            concatenated_raw += temp_9_raw[i + j]
                        datas[data_line[520]].append(concatenated)
                        raws[data_line[520]].append(concatenated_raw)
                        mfccs[data_line[520]].append(librosa.feature.mfcc(np.array(concatenated_raw), 512, n_mfcc=mfcc_size))

                    temp_9_fft = []
                    temp_9_raw = []

    datas_valid = [[], [], []]
    raws_valid = [[], [], []]
    mfccs_valid = [[], [], []]
    for file_name in glob.glob("data/12_10_right/valid/*.txt"):
        with open(file_name) as f:
            temp_9_fft = []
            temp_9_raw = []
            for linenum, line in enumerate(f):
                if linenum % 10 == 0:
                    continue
                data_line = [int(i) for i in line.split()]
                assert len(data_line) == 521
                normalized_fft = [x / MAX_MINDWAVE_VALUE for x in data_line[512:520]]
                normalized = [x / MAX_RAW for x in data_line[:512]]
                temp_9_fft.append(normalized_fft)
                temp_9_raw.append(normalized)
                if linenum % 10 == 9:
                    for i in range(9 - window_size + 1):
                        concatenated = []
                        concatenated_raw = []
                        for j in range(window_size):
                            concatenated += temp_9_fft[i + j]
                            concatenated_raw += temp_9_raw[i + j]
                        datas_valid[data_line[520]].append(concatenated)
                        raws_valid[data_line[520]].append(concatenated_raw)
                        mfccs_valid[data_line[520]].append(
                            librosa.feature.mfcc(np.array(concatenated_raw), 512, n_mfcc=mfcc_size))
                    temp_9_fft = []
                    temp_9_raw = []

    train_data_fft = np.array(datas[0] + datas[1] + datas[2], dtype="float64")
    train_data_raw = np.array(raws[0] + raws[1] + raws[2], dtype="float64")
    train_data_mfcc = np.array(mfccs[0] + mfccs[1] + mfccs[2], dtype="float64")
    train_label = np.array(
        [[1., 0., 0.]] * len(datas[0]) + [[0., 1., 0.]] * len(datas[1]) +[[0., 0., 1.]] * len(datas[2]))

    valid_data_fft = np.array(datas_valid[0] + datas_valid[1]+ datas_valid[2], dtype="float64")
    valid_data_raw = np.array(raws_valid[0] + raws_valid[1] + raws_valid[2], dtype="float64")
    valid_data_mfcc = np.array(mfccs_valid[0] + mfccs_valid[1] + mfccs_valid[2], dtype="float64")
    valid_label = np.array(
        [[1., 0., 0.]] * len(datas_valid[0]) + [[0., 1., 0.]] * len(datas_valid[1]) + [[0., 0., 1.]] * len(datas_valid[2]))

    return train_data_fft, train_data_raw, train_data_mfcc, train_label, valid_data_fft, valid_data_raw, valid_data_mfcc, valid_label



def train_model(model, train_data_fft, train_data_raw, train_data_mfcc, train_label, valid_data_fft, valid_data_raw, valid_data_mfcc, valid_label):
    checkpointer = ModelCheckpoint(filepath='weights.{epoch:02d}-{val_loss:.2f}.hdf5', verbose=1, save_best_only=True)
    model.fit(x=[train_data_fft, np.expand_dims(train_data_raw, axis=2), np.expand_dims(train_data_mfcc, axis=3)], y=train_label,
              validation_data=([valid_data_fft, np.expand_dims(valid_data_raw, axis=2), np.expand_dims(valid_data_mfcc, axis=3)], valid_label), epochs=40, shuffle=True, batch_size=16, callbacks=[checkpointer])

def test_model(model, valid_data_fft, valid_data_raw, valid_data_mfcc, valid_label):
    predicted = model.predict([valid_data_fft, np.expand_dims(valid_data_raw, axis=2), np.expand_dims(valid_data_mfcc, axis=3)])
    res = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
    res2 = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
    for idx, ans in enumerate(predicted):
        pred = np.argmax(ans)
        sol = np.argmax(valid_label[idx])
        res[sol][pred] += 1
        newans = 1
        if ans[0] > 0.65:
            newans = 0
        elif ans[2] > 0.5:
            newans = 2
        res2[sol][newans] += 1
        print("<<" + str(valid_label[idx]))
        print(ans)

    print(res)
    print(res2)

def main():
    train_data_fft, train_data_raw, train_data_mfcc, train_label, valid_data_fft, valid_data_raw, valid_data_mfcc, valid_label = load_data()
    model = prepare_model(window_size, mfcc_size)
    train_model(model, train_data_fft, train_data_raw, train_data_mfcc, train_label, valid_data_fft, valid_data_raw, valid_data_mfcc, valid_label)
    test_model(model, valid_data_fft, valid_data_raw, valid_data_mfcc, valid_label)

if __name__ == "__main__":
    main()