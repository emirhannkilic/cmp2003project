# User-Based Collaborative Filtering System (CMP2003)

A C++ implementation of a User-Based Collaborative Filtering (UBCF) recommendation system using Pearson Correlation similarity.

This project demonstrates efficient use of STL data structures, similarity computation, and top-k neighbor selection for rating prediction.

---

## Overview

The system predicts a user's rating for a target item based on ratings from similar users.

Main workflow:

- Compute similarity between users using Pearson Correlation
- Select top-k most similar neighbors
- Predict rating using weighted average of neighbor ratings

The implementation focuses on algorithmic efficiency and proper use of C++ STL containers.

---

## Algorithm Design

### Similarity Metric

Pearson Correlation Coefficient is used to measure similarity between users:

- Considers only co-rated items
- Normalizes rating bias
- Captures linear relationship between users

### Neighbor Selection

- Similarity scores are computed for all users
- `partial_sort` is used to extract top-k neighbors
- Avoids full sorting for better performance

### Rating Prediction

Predicted rating is computed using:

- Weighted average of neighbor ratings
- Weights determined by similarity score

---

## Technologies Used

- C++
- Standard Template Library (STL)
  - unordered_map
  - vector
  - pair
  - partial_sort
- File I/O

---

## Data Structures

The rating matrix is stored using:

```cpp
unordered_map<int, unordered_map<int, double>>
```

Similarity scores are stored using:

```cpp
vector<pair<int, double>>
```

Design emphasizes constant-time lookup and efficient memory usage.

---

## Time Complexity

- Similarity computation: O(U × I)
- Top-k selection: O(U log k)

Where:
U = number of users  
I = number of co-rated items  

---

## Academic Context

Developed for:
### CMP2003 – Data Structures and Algorithms | Bahçeşehir University

---

## Detailed Report

A full academic report describing design and mathematical background is available in `report.pdf`.
