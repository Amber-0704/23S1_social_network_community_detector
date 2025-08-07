# Social Network Community Detector

This is a C language implementation of a **community detection system** for a simplified social network. It processes user-friendship data and identifies connected communities using graph traversal algorithms.

## Features

- Parse user-friendship relationships from input files  
- Detect and count connected communities  
- Print individual community information  
- Designed with modularity in mind  
- Compatible with test input/output verification  

---

## Getting Started

### Compilation

Use `gcc` or any C compiler to build the program:

```bash
gcc program.c -o community_detector
```

or if you're using the separated module file:

```bash
gcc program.c c_community_detection.c -o community_detector
```

### Run

```bash
./community_detector < test1.txt
```

Expected output will be printed to standard output. You can compare it with the sample files like `test1-output.txt`.

---

## File Structure

```
.
├── program.c                 # Main implementation file
├── c_community_detection.c  # Modular logic for detecting communities
├── test0.txt                # Sample input file 0 (user/friendship pairs)
├── test0-output.txt         # Expected output for test0
├── test1.txt                # Sample input file 1
├── test1-output.txt         # Expected output for test1
├── README.md                # Project documentation (this file)
```

> `.DS_Store` is a macOS system file and can be ignored or deleted.

---

## Sample Input

`test1.txt` contains input in the following format:

```
0 1
1 2
3 4
```

This represents 2 communities:
- Community 1: 0-1-2  
- Community 2: 3-4  

---

## Output

Expected output:

```
Found 2 communities.
Community 1: 0 1 2
Community 2: 3 4
```

---

## Algorithms Used

- Graph construction via adjacency matrix or list  
- Community detection via **Depth-First Search (DFS)**  
- Efficient tracking using visited arrays and ID grouping  

---

