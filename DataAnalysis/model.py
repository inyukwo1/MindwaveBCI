import numpy as np
from keras.models import Sequential, Model
from keras.layers import Dense, Activation, Dropout, BatchNormalization, Input, Conv1D, Conv2D, Flatten, concatenate, add
from keras.metrics import categorical_accuracy
from random import shuffle


def prepare_model(window_size, mfcc_size):
    in_fft = Input(shape=(8 * window_size,))
    x_fft = Dense(8, input_dim=8, activation='relu')(in_fft)
    x_fft = Dense(8, input_dim=8, activation='relu')(x_fft)

    # model raw
    in_raw = Input(shape=(512 * window_size, 1))
    x_1 = Conv1D(4, 3, padding="same", activation="relu")(in_raw)
    x_raw = Conv1D(4, 3, padding="same", activation="relu")(x_1)
    x_raw = add([x_1, x_raw])
    x_raw = Conv1D(8, 3, strides=2, padding="same", activation="relu")(x_raw)
    x_raw = Dropout(0.5)(x_raw)
    x_raw = BatchNormalization()(x_raw)
    x_2 = Conv1D(32, 3, padding="same", activation="relu")(x_raw)
    x_raw = Conv1D(32, 3, padding="same", activation="relu")(x_2)
    x_raw = Dropout(0.5)(x_raw)
    x_raw = add([x_2, x_raw])
    # x_raw = Conv1D(64, 3, strides=2, padding="same", activation="relu")(x_raw)
    # x_raw = Dropout(0.5)(x_raw)
    # x_raw = Conv1D(128, 3, strides=2, padding="same", activation="relu")(x_raw)
    # x_raw = BatchNormalization()(x_raw)
    # # x_raw = Conv1D(128, 3, strides=2, padding="same", activation="relu")(x_raw)
    # # x_raw = Dropout(0.5)(x_raw)
    # # x_raw = Conv1D(64, 3, strides=2, padding="same", activation="relu")(x_raw)
    # x_raw = Dropout(0.5)(x_raw)
    # x_raw = BatchNormalization()(x_raw)
    x_3 = Conv1D(32, 3, padding="same", activation="relu")(x_raw)
    x_3 = Dropout(0.5)(x_3)
    x_raw = Conv1D(32, 3, padding="same", activation="relu")(x_3)
    x_raw = add([x_3, x_raw])
    x_raw = Conv1D(16, 3, strides=2, padding="same", activation="relu")(x_raw)
    x_raw = Flatten()(x_raw)
    x_raw = Dense(8, activation="relu")(x_raw)

    in_mfcc = Input(shape=(mfcc_size, window_size + 1, 1))
    x_mfcc = Conv2D(4, 3, padding="same", activation="relu")(in_mfcc)
    x_mfcc = Conv2D(8, 3, padding="same", activation="relu")(x_mfcc)
    x_mfcc = Dropout(0.5)(x_mfcc)
    x_mfcc = BatchNormalization()(x_mfcc)
    x_mfcc = Conv2D(16, 3, padding="same", activation="relu")(x_mfcc)
    x_mfcc = Dropout(0.5)(x_mfcc)
    x_mfcc = BatchNormalization()(x_mfcc)
    x_mfcc = Conv2D(8, 3, strides=2,  padding="same", activation="relu")(x_mfcc)
    x_mfcc = Conv2D(4, 3, strides=2, padding="same", activation="relu")(x_mfcc)
    x_mfcc = Flatten()(x_mfcc)
    x_mfcc = Dense(8, activation="relu")(x_mfcc)

    x = concatenate([x_raw, x_fft, x_mfcc])
    # x = Dense(16, activation="relu")(x)
    x = Dense(8, activation="relu")(x)
    out = Dense(3, activation="softmax")(x)

    # model_raw = Model(in_raw, out_raw)
    # model_raw.compile(optimizer='adam',
    #               loss='categorical_crossentropy',
    #               metrics=[categorical_accuracy])
    model = Model([in_fft, in_raw, in_mfcc], out)
    model.compile(optimizer='adam',
                  loss='categorical_crossentropy',
                  metrics=[categorical_accuracy])
    return model
