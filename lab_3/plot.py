import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
data = pd.read_csv('sorting_analysis.csv')

# Create a simple line plot
plt.figure(figsize=(10, 6))

# Plot average comparisons for each algorithm
plt.plot(data['Size'], data['Insertion_Avg_Comp'], label='Insertion Sort', marker='o')
plt.plot(data['Size'], data['Quick_Avg_Comp'], label='Quick Sort', marker='s')
plt.plot(data['Size'], data['Comb_Avg_Comp'], label='Comb Sort', marker='^')

# Add labels and title
plt.xlabel('Array Size')
plt.ylabel('Average Number of Comparisons')
plt.title('Sorting Algorithms Performance Comparison')
plt.legend()
plt.grid(True)

# Show the plot
plt.show()
