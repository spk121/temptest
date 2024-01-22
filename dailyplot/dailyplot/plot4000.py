from pathlib import Path
import dailyplot.parsecsv as pcsv
import matplotlib.pyplot as plt
import numpy as np

def plot_4000_x_vs_time(data):
    arr = np.array(data)
    tlabel = arr[0,0]
    t = arr[1:,0]
    x = arr[1:,1]
    y = arr[1:,2]
    z = arr[1:,3]

    fig, ax = plt.subplots(3,1)
    ax[0].plot(t,x)
    ax[0].set_title("4000 Position")
    ax[0].set_xlabel('Time (sec)')
    ax[0].set_ylabel('X (m)')
    ax[1].plot(t,y)
    ax[1].set_xlabel('Time (sec)')
    ax[1].set_ylabel('Y (m)')
    ax[2].plot(t,z)
    ax[2].set_xlabel('Time (sec)')
    ax[2].set_ylabel('Z (m)')
    plt.show()

    
def do_plot_4000():
    data = pcsv.read_csv_file('4000.csv')
    plot_4000_x_vs_time(data)

def plot_4000():
    my_file = Path("4000.csv")
    if my_file.is_file():
        do_plot_4000()
        return True
    return False

    