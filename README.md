<h1>NEAT AI</h1>
<p>NEAT AI is based off the research paper published by Kenneth O. Stanley and Risto Miikkulainen from the Department of Computer Sciences 
  at the University of Texas at Austin. The network learns via means of evolution taking inspiration from nature.</p>
<h2>GUI</h2>

![NEAT](https://github.com/SteveF42/NEAT/assets/54603983/1de0f0da-630b-47cf-ad0a-fcbe5878d8e9)
<p>The current GUI is currently powered by SFML. For setup ensure SFML is installed to compile the application. Download link can be found <a href="https://www.sfml-dev.org/download/sfml/2.6.1/">here.</a></p>
  <h2>XOR problem</h2>
  <p>XOR problem states the following</p>
  <ul>
    <li>0 ^ 0 = 0</li>
    <li>1 ^ 0 = 1</li>
    <li>0 ^ 1 = 1</li>
    <li>1 ^ 1 = 0</li>
  </ul>
  <p>This library is able to solve the given problem as fast as 30 generations.</p>
  <h2>Issues/ Improvements</h2>
  <p>Currently during the learning process the algorithm sometimes gets stuck in local minimums as it struggles to find an optimal solution.</p>
  <p>One improvement to be made can be to further strengthen the speciation. Currently, the older species are getting thrown out with no history of past generations.
   Implementing stagnation could be a great way to improve performance.</p>
   <p>I would also like to add some sort of parallelism to make the training run even faster.</p>

  <h2>Implementing the library</h2>
   <p>The library is based of an OOP, to add this to any library simply derive the NEAT class and implement the playGame and testNetwork methods.</p> 
  <h2>Setup</h2>
  <p>This project is based off MakeFile, simply replace the relative inlcude and lib path and run the makefile.</p>
