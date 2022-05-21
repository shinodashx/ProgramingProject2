# ProgramingProject2

This coursework, I choose the first route finding project. In this project, the function of finding the shortest path of any two points is realized, and visualize it. First, read the given map file into two `linked lists`, discretize all points, and use the `array-optimized adjacency list` to build a map. In the calculation of the shortest path, I used two algorithms. When there is no negative weight edge in the graph, the program will automatically select the `Dijkstra algorithm` optimized by `segment tree`, otherwise it will use the  `bellman-Ford algorithm` optimized by the `queue`. The algorithm finds the shortest path. In the program, three data structures of `array-optimized adjacency list`, `segment tree`, and `queue` are mainly used. The algorithm uses `merge sort`, `discretization`,` Dijkstra algorithm`， `bellmanFord` and `binary search`. This project uses SDL to display the map and the shortest path, and displays the `UI` through `libui`. The user can input the start and end points in the `UI` interface, and the shortest path between two points is displayed in real time. When drawing graphics, the user's mouse and keyboard can be monitored in real time to realize the zoom in, zoom out and drag of the map.

### version control

![](vc.png)

