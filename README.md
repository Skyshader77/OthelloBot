<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/Skyshader77/OthelloBot">
    <img src="OthelloApp/images/Othello_Spielbrett.jpg" alt="Logo" width="200" height="200">
  </a>
  <h3 align="center">Bothello</h3>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

The purpose of this project was to create a bot capable of playing Othello. It uses the alpha-beta pruning algorithm:

https://www.geeksforgeeks.org/artificial-intelligence/alpha-beta-pruning-in-adversarial-search-algorithms/

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* [![CPlusPlus][CPlusPlus]][CPlusPlus-url]
* [![Qt][Qt]][Qt-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites
1. GDB needs to installed on your machine.
* gdb
  ```sh
  sudo apt-get install build-essential gdb

  ```


2. QT Creator must be installed on your machine:

```sh
sudo apt-get -y install build-essential openssl libssl-dev libssl1.0 libgl1-mesa-dev libqt5x11extras5 '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev git zip unzip
```

### Installation


1. Clone the repo
   ```sh
   git clone https://github.com/Skyshader77/OthelloBot.git
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

### No UI
To run the application without the UI on Linux (for testing bots vs bots only):
```sh
   make clean
   ```

```sh
   make
   ```

```sh
   ./othello-bot
   ```
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### With UI
1. Open the project in QT Creator
2. Build and run the project.

<!-- ROADMAP -->
## Roadmap
- [X] Add Qt interface
- [X] Ensure the bot can reach a Max depth of 8
- [ ] Add a MCSTS algorithm

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[product-screenshot]: images/Othello_Spielbrett.jpg
[CPlusPlus]: https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white
[CPlusPlus-url]: https://isocpp.org/
[Qt]: https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white
[Qt-url]: https://www.qt.io/