*this document is written by MOU(a.k.a MARKDOWN editor)*

# Principles of Financial Computing

**Student**, Taekmin Kim(T04902206, tantara.tm@gmail.com)

**Environment**

- c++ 4.2.1
- Apple LLVM 7.0.2


## HW2 American-style Asian single-barrier up-and-out calls(~4/27)

### 1. Files

```
.
+-- README 		# documentation
|-- hw2.cpp 	# source code
`-- a.out 		# executable
```

### 2. TEST

```
# g++ hw2.cpp && ./a.out
```

#### INPUT - input.txt

``` 
100	# S = 100, X = 80, H = 130, t = 1 (years), s = 30%, r = 10%, n = 100, and k = 300
80
130
1
0.3
0.1
100
300
```

#### OUTPUT
```
25.6763				# (price)
```

### 3. Implementation

1. Calculate A_max, A_min
2. Calculate A
3. Calculate A_u, A_d, l, C_u, C_d
4. Update C


