import matplotlib.pyplot as plt
import numpy as np
import glob
import pickle

card_val_d = {
    '2': 2,
    '3': 3,
    '4': 4,
    '5': 5,
    '6': 6,
    '7': 7,
    '8': 8,
    '9': 9,
    'T': 10,
    'J': 11,
    'Q': 12,
    'K': 13,
    'A': 14,
}
situation_d = {
    "CO": 0,
    "DE": 1,
    "DE_CO": 2,
    "SB": 3,
    "SB_CO": 4,
    "SB_DE": 5,
    "SB_CO_DE": 6,
    "BB_CO": 7,
    "BB_DE": 8,
    "BB_SB": 9,
    "BB_CO_DE": 10,
    "BB_CO_SB": 11,
    "BB_DE_SB": 12,
    "BB_CO_DE_SB": 13,
}
player_d = {
    "CutOff": 0,
    "Dealer": 1,
    "SmallBlind": 2,
    "BigBlind": 3,
}
situation_player_d = {
    "CO": "CutOff",
    "DE": "Dealer",
    "DE_CO": "Dealer",
    "SB": "SmallBlind",
    "SB_CO": "SmallBlind",
    "SB_DE": "SmallBlind",
    "SB_CO_DE": "SmallBlind",
    "BB_CO": "BigBlind",
    "BB_DE": "BigBlind",
    "BB_SB": "BigBlind",
    "BB_CO_DE": "BigBlind",
    "BB_CO_SB": "BigBlind",
    "BB_DE_SB": "BigBlind",
    "BB_CO_DE_SB": "BigBlind"
}


def hand_str_2_idx(hand):
    M = hand[0]
    m = hand[1]
    suit = hand[2]
    M = card_val_d[M] - 2
    m = card_val_d[m] - 2
    suit = int(suit == 's')
    return M ** 2 + 2 * m + suit


def count_lines(filename):
    with open(filename, 'r') as file:
        return sum(1 for _ in file)


def find_block_length(file):
    file = open(file)
    block_length = 0
    flag = False
    for i, line in enumerate(file):
        if flag:
            block_length += 1
        if "Summary" in line:
            if flag:
                break
            else:
                flag = True
    return block_length


def parse_mwu_line(split_line):
    values = [float(i) for i in split_line[1:-1]]
    return values


def parse_bandit_line(split_line):
    values = []
    for idx, entry_values in enumerate(split_line[1:-1]):
        entry_values_split = entry_values.split(" ")
        sign = entry_values_split[1]
        diff = float(entry_values_split[2])
        if sign == "A":
            sign = 1
        elif sign == "F":
            sign = -1
        else:
            raise ValueError("Invalid sign")
        values.append(sign * diff)
    return values


def add_initial_values_mwu(data, v):
    array = np.ones((4, 1, 14, 169)) * v
    new_arr = np.concatenate((array, data), axis=1)
    return new_arr


def parse_log(file, parser, v):
    block_length = find_block_length(file)
    N = round(count_lines(file) / block_length)
    file = open(file)
    data_results_ = np.zeros([4, N, 14, 169])
    curr_t = -1
    curr_player = -1
    for i, line in enumerate(file):
        if "22o" in line:
            curr_player += 1
            curr_t += 1

        split_line = line.split("|")
        if len(split_line) == 16:
            if split_line[0][0] == " ":
                continue
            else:
                assert curr_player != -1
                hand_idx = hand_str_2_idx(hand=split_line[0])
                data_results_[curr_player % 4, curr_t // 4, :, hand_idx] = parser(split_line=split_line)

    data_results_ = add_initial_values_mwu(data_results_, v=v)
    return data_results_


def get_data(path, parser, init_v):
    f_data_results = []
    files = glob.glob(path)
    for filename in files:
        data_res = parse_log(filename, parser, v=init_v)
        f_data_results.append(data_res)

    min_t = min([data_res.shape[1] for data_res in f_data_results])
    for i, data_res in enumerate(f_data_results):
        f_data_results[i] = data_res[:, :min_t, :, :]
    f_data_results = np.array(f_data_results)
    return f_data_results


if __name__ == "__main__":

    data_results = get_data(path="MWU_*_10T.log", parser=parse_mwu_line, init_v=0.5)

    mean_data_results = np.mean(data_results, axis=0)
    std_data_results = np.std(data_results, axis=0)

    sit = "BB_DE"
    for hand in ['K6s', 'K7s', 'K6o']:
        t = range(data_results.shape[2])
        m = mean_data_results[player_d[situation_player_d[sit]], :, situation_d[sit], hand_str_2_idx(hand)]
        s = std_data_results[player_d[situation_player_d[sit]], :, situation_d[sit], hand_str_2_idx(hand)]
        plt.plot(t, m, label=hand)
        plt.fill_between(t, m-s, m+s, color='gray', alpha=0.2)

    plt.title("Data")
    plt.legend()
    plt.grid()
    plt.xlabel("#Iteration * 10**7")
    plt.ylabel("<P>")
    plt.show()



