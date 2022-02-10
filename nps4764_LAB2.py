# Nicholas Soliz
# UTA ID 1001744764
# 09 November 2020
# Windows

    # Opened the file to read in the expressions
filename = open("input_RPN.txt","r")
nums = []
    # Read the expressions and made an array of each element of the expression
for x in filename:
    i = 0
    y = 0
    for c in x:
        nums.append(x[i])
        i = i + 1

        # Individually checked each element of the expression,
        # if the element was an operation to be performed
        # then the previous two elements were operated on
        # then the array was updated to show the answer of the
        # operation and remove the used operator, then read
        # until a new operation and repeating until no more
        # operators existed and an answer was reached.
        # Also any non number or non-operator character was
        # removed from the array and treated as if they did not exist.

    for g in nums:
        i = 0
        k = 0
        l = len(nums)
        while l > k: # Iterating until the end of the array has been reached
            if(nums[i] == "/n"):
                nums.remove(nums[i])
                i = i -1
            elif(nums[i] == " "): # Removal of blank spaces
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "("):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == ")"):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "+"):  # Addition operation
                n = int(nums[i - 1])
                v = int(nums[i - 2])
                y = (v + n)
                nums[i] = y
                nums.pop(i-1)   # Removing the old values from the array
                nums.pop(i-2)   # Removing the old values from the array
                i = i-2
            elif (nums[i] == "*"):  # Multiplication operation
                n = int(nums[i - 1])
                v = int(nums[i - 2])
                y = (v * n)
                nums[i] = y
                nums.pop(i-1)   # Removing the old values from the array
                nums.pop(i-2)   # Removing the old values from the array
                i = i-2
            elif (nums[i] == "-"):  # Subtraction operation
                n = int(nums[i - 1])
                v = int(nums[i - 2])
                y = (v - n)
                nums[i] = y
                nums.pop(i-1)   # Removing the old values from the array
                nums.pop(i-2)   # Removing the old values from the array
                i = i-2
            elif (nums[i] == "/"):  # Division operation
                n = int(nums[i - 1])
                v = int(nums[i - 2])
                y = (v / n)
                nums[i] = y
                nums.pop(i-1)   # Removing the old values from the array
                nums.pop(i-2)   # Removing the old values from the array
                i = i-2

                    # Removal of other random characters
            elif (nums[i] == "!"):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "@"):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "#"):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "$"):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "%"):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "^"):
                nums.remove(nums[i])
                i = i -1
            elif (nums[i] == "&"):
                nums.remove(nums[i])
                i = i -1
            i = i+1
            k = k+1

        # A print statement to show the answer to the user
    print(x, " = ", y)
    nums.clear()

filename.close() # Closing the opened file to save memory
