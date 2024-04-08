### Input Format:
```
MAX_RUNTIME time
MAX_LOAD load
GRID_SIZE p
CAPACITY C
PINS numPins
PIN index0 x0 y0
PIN index1 x1 y1
…
TAPS numTaps
TAP index0 x0 y0
TAP index1 x1 y1
…
```
#### Input Formal Description:

```MAX_RUNTIME time```  Maximum runtime (s)

```MAX_LOAD load```  Maximum number of pins driven by a tap

```GRID_SIZE p```  Size of grid graph

```CAPACITY C```  Capacity of each edge in the grid graph

```PINS num```  Number of clock pins
followed by the definitions of clock pins

```PIN index x y```  index specifies the pin index
x and y specify the pin position

```TAPS num```  Number of clock taps
followed by the definitions of clock taps

```TAP index x y```  index specifies the tap index
x and y specify the tap position

### Output Format:
```
TAP index0
PINS num
PIN index0
PIN index1
…
ROUTING numEdges
EDGE x0 y0 x0’ y0’
EDGE x1 y1 x1’ y1’
…
TAP index1
PINS num
PIN index0
PIN index1
…
ROUTING numEdges
EDGE x0 y0 x0’ y0’
EDGE x1 y1 x1’ y1’
…
…
```

#### Output Format Description:
```TAP index```  The tap index

```PINS num```  The number of pins connected
by the tap

```PIN index```  The pin index

```ROUTING numEdges```  Number of edges in the routing
solution

```EDGE x y x’ y’```  (x, y) and (x’, y’) specify the two
ends of an edge. Overlapped edges
will be merged automatically.
