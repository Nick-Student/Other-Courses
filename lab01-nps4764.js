// Nicholas Soliz
// 1001744764
// 16 OCtober, 2020


// Question 1: Create an Array named inputtable with values 1-10
//==============================================================

var inputtable = [1,2,3,4,5,6,7,8,9,10];
console.log("Question 1: inputtable = ",inputtable);

// These values aare used to help me increment through the array, 0 for the first value in the array, 9 for the 10th value of the array
var begin = 0;
var end = 9;

// I made these placeholder arrays to be overwritten by transformations of the inputtable array.
var fHold = [];
var tHold = [];
var sHold = [];
var oHold = [];
var eHold = [];



// Question 2 : Use inputtable to create the following
//====================================================

    // a. Set of Multiples of 5 between 1 and 51 named fiveTable
        // For this function, I made a recursive call to continuously move through all elements of the array and to overwrite fHold with the values
        // of inputtable but with a scalar of 5.
function makeFive(begin, inputtable, end, fHold){
    if (begin > end)
        return fHold;
    else{
        fHold[begin] = inputtable[begin] * 5;
        return makeFive(begin + 1, inputtable, end, fHold)
    }
}

    // b. Set of Multiples of 13 between 1 and 131 named thirteenTable
        // For this function, I made a recursive call to continuously move through all elements of the array and to overwrite tHold with the values
        // of inputtable but with a scalar of 13.
function makeThirteen(begin, inputtable, end, tHold){
    if (begin > end)
        return tHold;
    else{
        tHold[begin] = inputtable[begin] * 13;
        return makeThirteen(begin + 1, inputtable, end, tHold)
    }
}

    // c. Set of squares of the values in inputtable named squaresTable
        // For this function, I made a recursive call to continuously move through all elements of the array and to overwrite sHold with the squares 
        // of each value in the array of inputtable. 
function makeSquare(begin, inputtable, end, sHold){
    if (begin > end)
        return sHold;
    else{
        sHold[begin] = inputtable[begin] * (begin+1);
        return makeSquare(begin + 1, inputtable, end, sHold)
    }
}

// The following three declarations are making arrays equal to the outputs of the recursive functions above. Paired with their mathematical equivalents above.

fiveTable = makeFive(begin, inputtable, end, fHold);
console.log("Question 2a : fiveTable = ", fiveTable);

thirteenTable = makeThirteen(begin, inputtable, end, tHold);
console.log("Question 2b : thirteenTable = ", thirteenTable);

squaresTable = makeSquare(begin, inputtable, end, sHold);
console.log("Question 2c : squaresTable = ", squaresTable);


// Question 3: Get the odd multiples of 5 between 1 and 100
//=========================================================
function makeOddFive(begin, fiveTable, end, oHold){
    if (begin > end)
        return oHold;
    else{
        oHold[begin] = fiveTable[begin] + (begin * 5);
        return makeOddFive(begin+1, fiveTable, end, oHold)
    }
}

oddFives = makeOddFive(begin, fiveTable, end, oHold);
console.log("Question 3: oddFives = ", oddFives);


// Question 4: Get the sum of even multiples of 7 between 1 and 100
//=================================================================

var counter = 0;
var oneHunna = 100; 
// This function finds the even numbers divisible by seven
function makeEvenSeven(begin, eHold, oneHunna, counter){
    if (counter+14 > oneHunna)
        return eHold;
    else{
        eHold[begin] = counter + 14;
        return makeEvenSeven(begin+1, eHold, oneHunna, counter+14);
    }
}

evenSevens = makeEvenSeven(begin, eHold, oneHunna,counter);

var total = 0;
var done = evenSevens.length;
// This function adds the numbers of the array together to get the sum
function sumThem(evenSevens, begin, total, done){
    if (begin > done-1){
        return total;
    }
    else{
        total = total + evenSevens[begin];
        return sumThem(evenSevens, begin+1, total, done);
    }
}

sevensSum = sumThem(evenSevens, begin, total, done);
console.log("Question 4: The sum of all even numbers that are multiples of seven but lower than 100 is: ",sevensSum);

// Question 5: Use currying to rewrite the function below: -
        /*function cylinder_volume(r, h){
                var volume = 0.0;
                volume = 3.14 * r * r * h;
                return volume;
            }
            Use r = 5 and h = 10 to call your curried function.
        */

// This passes the variables into the function one at a time, operating on them one at a time, until ultimately creating the complete function.
var cylinder_volume = r => h => (3.14*r*r*h);

test = cylinder_volume(5)(10);
console.log("Question 5: The volume of a cylinder with a radius of 5 and a height of 10 = ",test);

// Question 6: Use the following code to take advantage of closures to wrap content with HTML tags,
// specifically show an HTML table consisting of a table row that has at least one table cell/element.
// You can use the console to output your results.

makeTag = function(beginTag, endTag){
    return function(textcontent){
        return beginTag + textcontent + endTag;
    }
}    

// This set of lines is to set the begin and end tag for each pass through the makeTag function
makeTable = makeTag("<Table>\n","\n<Table>");
makeTr = makeTag("\t<tr>\n","\n\t<tr>\n");
makeTh = makeTag("\t\t<th> "," <th>\n");

// Blank arrays to be filled
var required = [];
var input = [];
var otherput = [];
var passTwo = [];
var finalPass = [];

// My questions asked to the user, stored in an array and passed into the makeTh function, which is function(textcontent), so we can put the <th> on either side of our input
required[0] = makeTh("Favorite Color");
required[1] = makeTh("Favorite Animal");
required[2] = makeTh("Favorite Food");

// Input answers from user, stored in an array and passed into the makeTh function, which is function(textcontent), so we can put the <th> on either side of our input
input[0] = makeTh("Rainbow fish black");
input[1] = makeTh("Alaskan Venom-beaked Hunting Hornet");
input[2] = makeTh("Crisco");

// Second input set, stored in an array and passed into the makeTh function, which is function(textcontent), so we can put the <th> on either side of our input
otherput[0] = makeTh("Red");
otherput[2] = makeTh("Horse");
otherput[3] = makeTh("Red Horse Burgers");

// This is we pass the already <th> tagged strings back into function(textcontent) as a parameter in order to make it so the <tr> tag  is placed correctly. I have no worldly idea 
// why i am getting commas
passTwo[0] = makeTr(required);
passTwo[1] = makeTr(input);
passTwo[2] = makeTr(otherput);

// This is the final pass into the makeTag function, this takes out <tr> <th> bound content and gives it the final <table> tag. Then stores it all in a single variable.
finalPass = makeTable(passTwo);

console.log("Question 6:\n",finalPass);