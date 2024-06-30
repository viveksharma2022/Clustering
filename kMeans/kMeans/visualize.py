import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

if __name__=="__main__":
    fileName = r"C:\Users\vivek\Documents\GitHub\general_practice\clustering\Mall_Customers_classified.csv"
    
    data = pd.read_csv(fileName)

    plt.figure(1)
    plt.scatter(data['Age'], data[' Spending score(0-100)'], c = data[' kClass'])
    plt.show(block = True)
    pause = 1
