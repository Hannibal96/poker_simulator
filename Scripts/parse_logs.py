import matplotlib.pyplot as plt
import numpy as np
import os


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


def parse_mwu_log(file):
    N = round(count_lines(file) / 744)
    file = open(file)
    data_results = np.zeros([4, N, 14, 169])
    curr_t = -1
    curr_player = -1
    for i, line in enumerate(file):
        if "Position: CutOff" in line:
            curr_player = 0
            curr_t += 1
        if "Position: Dealer" in line:
            curr_player = 1
        if "Position: SmallBlind" in line:
            curr_player = 2
        if "Position: BigBlind" in line:
            curr_player = 3

        split_line = line.split("|")
        if len(split_line) == 16:
            if split_line[0][0] == " ":
                continue
            else:
                hand_idx = hand_str_2_idx(hand=split_line[0])
                values = [float(i) for i in split_line[1:-1]]
                data_results[curr_player, curr_t, :, hand_idx] = values

    return data_results


def parse_bandit_log(file):
    N = round(count_lines(file) / 744)
    file = open(file)
    data_results = np.zeros([4, N, 14, 169])
    curr_t = -1
    curr_player = -1
    for i, line in enumerate(file):
        if "Position: CutOff" in line:
            curr_player = 0
            curr_t += 1
        if "Position: Dealer" in line:
            curr_player = 1
        if "Position: SmallBlind" in line:
            curr_player = 2
        if "Position: BigBlind" in line:
            curr_player = 3

        split_line = line.split("|")
        if len(split_line) == 16:
            if split_line[0][0] == " ":
                continue
            else:
                hand_idx = hand_str_2_idx(hand=split_line[0])

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
                    data_results[curr_player, curr_t, idx, hand_idx] = sign * diff


    return data_results




def mwu_data():
    f_data_results = []
    for filename in os.listdir("./"):
        if filename.startswith("MWU"):
            data_res = parse_mwu_log(filename)
            f_data_results.append(data_res)

    min_t = min([data_res.shape[1] for data_res in f_data_results])
    for i, data_res in enumerate(f_data_results):
        f_data_results[i] = data_res[:, :min_t, :, :]
    f_data_results = np.array(f_data_results)
    return f_data_results


def bandit_data():
    f_data_results = []
    for filename in os.listdir("./"):
        if filename.startswith("bandit"):
            data_res = parse_bandit_log(filename)
            f_data_results.append(data_res)

    min_t = min([data_res.shape[1] for data_res in f_data_results])
    for i, data_res in enumerate(f_data_results):
        f_data_results[i] = data_res[:, :min_t, :, :]
    f_data_results = np.array(f_data_results)
    return f_data_results


data_results = bandit_data()
mean_data_results = np.mean(data_results, axis=0)
std_data_results = np.std(data_results, axis=0)

for hand in ['72o', '62o', '54o']:
    t = range(data_results.shape[2])
    m = mean_data_results[player_d["Dealer"], :, situation_d["DE"], hand_str_2_idx(hand)]
    s = std_data_results[player_d["Dealer"], :, situation_d["DE"], hand_str_2_idx(hand)]
    plt.plot(t, m, label=hand)
    plt.fill_between(t, m-s, m+s, color='gray', alpha=0.2)

plt.title("Data")
plt.legend()
plt.grid()
plt.xlabel("#Iteration * 10**7")
plt.ylabel("<P>")
plt.show()





0/0


data_results = mwu_data()

mean_data_results = np.mean(data_results, axis=0)
std_data_results = np.std(data_results, axis=0)

for hand in ['K5s']:
    t = range(data_results.shape[2])
    m = mean_data_results[player_d["Dealer"], :, situation_d["DE"], hand_str_2_idx(hand)]
    s = std_data_results[player_d["Dealer"], :, situation_d["DE"], hand_str_2_idx(hand)]
    plt.plot(t, m, label=hand)
    plt.fill_between(t, m-s, m+s, color='gray', alpha=0.2)

plt.title("Data")
plt.legend()
plt.grid()
plt.xlabel("#Iteration * 10**7")
plt.ylabel("<P>")
plt.show()



