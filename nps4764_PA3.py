# Nicholas Soliz
# UTA ID 1001744764
# 02 December 2020
# Windows

# open that file up to be used as input
filename = open("input.txt","r")

# a variable to be used as a counter for block depth
i = 0

# we loop though each line of the file, dealing with each line one by one
for x in filename:
    if ( "//" in x ):       # if we see a comment, we don't need to deal with incrementation or decrementation
        print( i , x )          # print the line with the current block depth
    elif ( "print" in x):   # if we see a print statement then we won't be incrementing or decrementing
        print( i , x )          # print the line with the current block depth
    elif ( "{" in x ):      # if we see an opening brace we know to increment i, this shows additional depth
        i = i + 1               # perform the incrementation
        print( i , x )          # print the current depth and the line 
    elif ( "}" in x ):      # if we see a closing brace we know we are moving out of a block and block depth decrements
        print( i , x )          # print the line prior to decrementation 
        i = i - 1               # decrement the block depth
    else:                   # all other cases
        print( i , x)           # print the line with the current block depth

filename.close()            # close that file son, save some memory for the rest of us to use
