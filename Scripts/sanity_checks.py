import numpy as np
import glob
import matplotlib.pyplot as plt


if __name__ == "__main__":
    data = []
    for file_name in glob.glob("./10T/M*.log"):
        stats_t = []
        stats = np.zeros(9)
        file = open(file_name)
        for line in file:
            if line.startswith("HighCard"):
                stats[0] = float(line.split(":")[1])
            elif line.startswith("StraightFlush"):
                stats[8] = float(line.split(":")[1])
                stats_t.append(stats.copy())
            elif line.startswith("Pair"):
                stats[1] = float(line.split(":")[1])
            elif line.startswith("TwoPairs"):
                stats[2] = float(line.split(":")[1])
            elif line.startswith("Trips"):
                stats[3] = float(line.split(":")[1])
            elif line.startswith("Straight"):
                stats[4] = float(line.split(":")[1])
            elif line.startswith("Flush"):
                stats[5] = float(line.split(":")[1])
            elif line.startswith("FullHouse"):
                stats[6] = float(line.split(":")[1])
            elif line.startswith("Quads"):
                stats[7] = float(line.split(":")[1])
        data.append(np.array(stats_t))
    data = np.array(data)
    mu = data.mean(axis=0)
    for i in range(9):
        plt.plot(mu[:, i])
        print(mu[-1, i])
        plt.grid()
        plt.show()




