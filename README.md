 # OneStraightCut
### One Cut Theorem | The Fold-And-Cut Problem
OneStraightCut is a computational origami project that implements an algorithm using the Straight Skeleton of a Polygon to calculate the Crease Pattern. The project is using CGAL (Computational Geometry Algorithms Library). This theorem enables cutting any straight-line drawing from a piece of paper using a single straight cut.
For more information on the One Cut Theorem and the Straight Skeleton Method, please refer to [this paper by Erik Demaine](https://erikdemaine.org/papers/JCDCG98/) :

---

## Features
- **Straight Skeleton Computation**: Generate the straight skeleton from a planar graph.
- **Crease Pattern**: Perpendiculars are added to make the Graph flat foldable and generate the Crease Pattern.
- **GUI**: A simple GUI to interact with the program and enter polygons.
- **PDF and PNG Generation** Export the Crease Pattern as a PDF or PNG file.

---

## Requirements
Ensure the following dependencies are installed before proceeding:
- **Conda**: Version 24.11.0
- **CMake**: Version 3.29 or higher
- **CGAL**: Version 6.0.1
- **C++**: A Compiler supporting C++20 or higher (e.g. Clang, GCC)
- **Python**: Version 3.12 or higher

It is possible that older versions may also work, but it has only been tested for the versions above.

---

## Installation
### Step 1: Clone the Repository
```bash
git clone https://github.com/LB-Lukas/OneCut-StraightSkeleton.git
cd OneCut-StraightSkeleton
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
You can also specify if you want to build the tests by adding ```-BUILD_TESTS=OFF``` to the cmake command.
This will skip the test build and reduce the build time.

---

## Running the Program
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

## Running the Tests
Our project uses Google Test for unit testing.
First make sure you have built the tests [see Step 3](#Step-3-Build-the-Project).
To run the tests, first activate the Conda environment:
```bash
conda activate onecut
```
Then you can run each test individually (e.g. for Perpendicular Test):
```bash
./build/tests/perpendicular_test
```

---
## Usage Guide
### Interacting with the GUI
The OneStraightCut application provides an interactive graphical user interface for defining and manipulating polygons. Below is a step-by-step guide on how to use the main functionalities.

#### Creating and Editing Polygons ####

- Placing Points: Hold Shift and Left Click anywhere on the canvas to place a point. Points are connected sequentially to form a polygon.
- Completing the Polygon: To finish drawing a polygon, either:

    - Click the "Finish Polygon" button, or

    - Right Click anywhere on the canvas.

#### Selecting Elements ####

- Selecting a Point or an Edge:

    - Left Click on a point to select it.

    - Left Click on an edge to select it.

#### Modifying the Polygon ####

- If a Point is Selected:

    - Move the point by dragging it with the mouse (holding Left Click).

    - Delete the point.

    - Delete the entire polygon.

- If an Edge is Selected:

    - Add a new point at the midpoint of the edge.

    - Delete the entire polygon.

#### Navigating the Canvas ####

- Zooming: Hold Ctrl and use the Mouse Wheel to zoom in or out.

- Panning: Click and hold the Mouse Wheel to move the canvas.

#### Display Options ####

You can toggle between different visualization modes:

- Display only the polygon.

- Show the computed straight skeleton.

- Show the fold pattern.

- Show the perpendiculars.

- Display both the skeleton and the perpendiculars.

#### File Operations ####

- Open and Save Files:

    - The application supports the ```.fold``` file format.

    - Use the "Open" and "Save" options to manage your work.

- Exporting:

    - You can export the current canvas as a ```.png``` or ```.pdf``` file for external use.

Inteface with Heart: ![Screenshot of GUI](/screenshot1.png "GUI")
