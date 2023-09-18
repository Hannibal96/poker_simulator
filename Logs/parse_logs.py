import glob
import os


CARDS = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A']
STATE = ["CO", "DE", "DE_CO", "SB", "SB_CO", "SB_DE", "SB_CO_DE", "BB_CO", "BB_DE", "BB_SB", "BB_CO_DE", "BB_CO_SB", "BB_DE_SB", "BB_CO_DE_SB"]
TAB_SPACE = 8


def extract_tables(logger):
    last_line = None
    with open(log, 'r') as f:
        for idx, line in enumerate(f):
            if "============================******************************************************============================" in line:
                last_line = idx

    with open(log, 'r') as f:
        content = f.read()
    res = content.split("\n")[last_line:]

    tabels_line = []
    for idx, line in enumerate(res):
        if "22o" in line:
            tabels_line.append(idx)

    co_idx, de_idx, sb_idx, bb_idx = tabels_line
    co_table = "\n".join(res[co_idx:co_idx + 169])
    de_table = "\n".join(res[de_idx:de_idx + 169])
    sb_table = "\n".join(res[sb_idx:sb_idx + 169])
    bb_table = "\n".join(res[bb_idx:bb_idx + 169])

    return co_table, de_table, sb_table, bb_table


def create_res_dict():
    res_dict = {}
    for idx_a, card_a in enumerate(CARDS):
        for idx_b, card_b in enumerate(CARDS[0:idx_a+1]):
            for suit in ['o', 's']:
                if suit == 's' and card_a == card_b:
                    continue
                hand = card_a+card_b+suit
                for state in STATE:
                    res_dict[(hand, state)] = 0
    return res_dict


def parse_table(table, indices, states, res_dict):
    for line in table.split("\n"):
        entries = line.split("\t")
        hand = entries[0]
        for i, s in zip(indices, states):
            if "A" in entries[i]:
                res_dict[(hand, s)] += 1
            elif "F" in entries[i]:
                continue
            else:
                assert False
    return res_dict


def print_dict(res_dict, n):
    print("\t", end="")
    for idx, state in enumerate(STATE):
        end = "\t" * (2 - len(state) // TAB_SPACE)
        print(state, end=end)
    print(end="\n")
    for idx_a, card_a in enumerate(CARDS):
        for idx_b, card_b in enumerate(CARDS[0:idx_a+1]):
            for suit in ['o', 's']:
                if suit == 's' and card_a == card_b:
                    continue
                hand = card_a+card_b+suit
                print(hand, end="\t")
                for state in STATE:
                    val = round(100 * res_dict[(hand, state)]/n, 3)
                    if len(str(val)) >= TAB_SPACE:
                        end = "\t"
                    else:
                        end = "\t\t"
                    print(val, end=end)
                print(end="\n")


directory = "./"
suffix = "*.log"


if __name__ == '__main__':
    res_dict = create_res_dict()
    for log_idx, log in enumerate(glob.glob(os.path.join(directory, suffix))):
        co_table, de_table, sb_table, bb_table = extract_tables(logger=log)

        parse_table(table=co_table, indices=[1], states=["CO"], res_dict=res_dict)
        parse_table(table=de_table, indices=[2, 3], states=["DE", "DE_CO"], res_dict=res_dict)
        parse_table(table=sb_table, indices=[4, 5, 6, 7], states=["SB", "SB_CO", "SB_DE", "SB_CO_DE"], res_dict=res_dict)
        parse_table(table=bb_table, indices=[8, 9, 10, 11, 12, 13, 14], states=["BB_CO", "BB_DE", "BB_SB", "BB_CO_DE", "BB_CO_SB", "BB_DE_SB", "BB_CO_DE_SB"], res_dict=res_dict)

        print_dict(res_dict=res_dict, n=log_idx+1)
        print("\n")

