Explanation of our project structure 
------------------------------------

* headers directory contains header files. 
* source directory contains source code. 
* outfiles/source directory is where the compile programs are stored when 
    using compile.sh 
* test directory is where images to be supplied to the programs must be stored. 
* results for storing results from m_dartboard_detector.cpp 
* hough_spaces stores hough spaces. 
* gradient_images stores gradient images. 

File Naming System
------------------
* Any cpp files which start with "m_" contain a main function. 


Compiling and Running a Program
-------------------------------
* To compile a program, we have provided a simple bash scriptl: compile.sh 
    * To run this, navigate to the root directory. 
    * then run the commadn "bash compile.sh source/file" where the filename
         does NOT have the cpp extension.
    * This then compiles the program in outfiles/source/
* To run a compiled program: 
    * Navigate to the root level of the directory. 
    *  run the command "outfiles/source/file.out " 
    * One can load images as a parameter, as the current url is set at the root
     level of the project. 

* Please note that our main classifier program is m_dartboard_detector and takes an image as a parameter. Storing its out put in the results directory. If no parameter is supplied, then the program will use all images to calculate the f1-score.

* m_tpr_calculator takes two parameters, firstly an integer specifying number of files provided, and secondly, the file names.  
