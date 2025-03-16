 # **OneStraightCut**
### One Cut Theorem | The Fold-And-Cut Problem
OneStraightCut is a computational origami project that implements an algorithm using the Straight Skeleton of a Polygon to calculate the Crease Pattern. The project is using CGAL (Computational Geometry Algorithms Library). This theorem enables cutting any straight-line drawing from a piece of paper using a single straight cut.
For more information on the One Cut Theorem and the Straight Skeleton Method, please refer to the following [paper by Erik Demaine](https://erikdemaine.org/papers/JCDCG98/) :
```BibTeX
@inproceedings{demaine2000folding,
  title={Folding and cutting paper},
  author={Demaine, Erik D and Demaine, Martin L and Lubiw, Anna},
  booktitle={Discrete and Computational Geometry: Japanese Conference, JCDCG’98 Tokyo, Japan, December 9-12, 1998. Revised Papers},
  pages={104--118},
  year={2000},
  organization={Springer}
}
``` 

---

## **Features**
- **Straight Skeleton Computation**: Generate the straight skeleton from a planar graph.
- **Crease Pattern**: Perpendiculars are added to make the Graph flat foldable and generate the Crease Pattern.
- **GUI**: A simple GUI to interact with the program and enter polygons.
- **PDF and PNG Generation** Export the Crease Pattern as a PDF or PNG file.

---

## **Requirements**
Ensure the following dependencies are installed before proceeding:
- **Conda**: Version 24.11.0
- **CMake**: Version 3.29 or higher
- **CGAL**: Version 6.0.1
- **C++**: A Compiler supporting C++20 or higher (e.g. Clang, GCC)
- **Python**: Version 3.12 or higher

---

## **Installation**
### Step 1: Clone the Repository
```bash
git clone https://github.com/OsterfeldTom/OneStraightCut.git
cd OneStraightCut
```
You are now in the project root directory. All further commands should be run from this location.

### Step 2: Create Conda Environment
Use these commands to create a Conda Environment named "onecut" and activate it.
```bash
conda env create -f onecut_requirements.yml
conda activate onecut
```

### Step 3: Build the Project
To be able to build the project you need to have CGAL installed on your system. If you have not installed CGAL yet, you can find the installation instructions [here](https://doc.cgal.org/latest/Manual/usage.html#title4) and [here](https://doc.cgal.org/latest/Manual/installation.html).

Build the project using CMake:
```bash
cmake -S . -B build && cmake --build build && cmake --install build
```
Depending on how you installed CGAL, you may need to specify the path to the library in the CMake command.
```bash
cmake -DCGAL_DIR=<Path to CGAL> -S . -B build && cmake --build build && cmake --install build
```
Replace `<Path to CGAL>` with the directory containing the CGAL library, in my case the command looks like this:
```bash
cmake -DCGAL_DIR=$HOME/cgal/CGAL-6.0.1/ -S . -B build && cmake --build build && cmake --install build
```
After succesfully building the project, there should be an file with the prefix "geometry" and the extension ".so" in the /python/gui/ directory. This file is the Python binding for the C++ library.

---

## **Running the Program**
To run the program first activate the Conda environment:
```bash
conda activate onecut
```
Then run the main.py file:
```bash
python3 python/gui/main.py
```
This will open the GUI Window.

---

## **~~Running the Tests~~**
~~Our project uses Google Test for unit testing.~~
~~To run the tests, first activate the Conda environment:~~
```bash
conda activate onecut
```
~~Then you can run each test individually (e.g. for Perpendicular Test):~~
```bash
./build/tests/perpendicular_test
```

