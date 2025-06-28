## ZAPPY Project – Multiplayer Network Game with Artificial Intelligence

### General Overview

**ZAPPY** is a real-time multiplayer network game developed over the course of a one-month team project. Inspired by games such as _Corewar_, ZAPPY involves teams of autonomous AI players competing to reach a shared objective: **raise at least 6 players from the same team to level 8** through a complex collaborative ritual system.

The project is architected around three main components:

-   **zappy\_server**: the game server, written in **C**.
-   **zappy\_gui**: the graphical client (spectator interface), developed in **C++**.
-   **zappy\_ai**: autonomous AI clients, developed in any programming language.

### Project Objectives

-   Build a scalable **client-server architecture** supporting simultaneous AI players and at least one GUI client.
-   Design a **custom text-based communication protocol** to handle actions, events, and state updates in real time.
-   Simulate a **dynamic game world** where AI players must manage resources, collaborate, and coordinate to perform rituals.
-   Create a **graphical interface** to visualize all game events and world state.

### How to Launch the ZAPPY Binaries

Each component of the ZAPPY project is compiled as an independent binary. To run the game, you must first launch the **server**, then connect one or more **AI clients** and optionally a **graphical client (GUI)**. Here are the execution instructions for each binary:

1\. Launch the Server (`zappy_server`)
```bash
./zappy_server -p <port> -x <width> -y <height> -n <team1> <team2> [...] -c <clients_per_team> -f <frequency>
```

**Arguments:**

-   `-p <port>`: TCP port to listen on
-   `-x <width>`: map width
-   `-y <height>`: map height
-   `-n <team1> <team2> [...]`: list of team names
-   `-c <clients_per_team>`: number of available slots per team
-   `-f <frequency>`: number of time units per second (higher = faster game)

**Example:**
```bash
./zappy_server -p 4242 -x 10 -y 10 -n Alpha Beta -c 6 -f 100
```

2\. Launch the Graphical Client (`zappy_gui`)
```bash
./zappy_gui -p <port> -h <server_ip>
```

**Arguments:**

`-p <port>`: same port as the server
`-h <server_ip>`: IP address of the server (e.g., `127.0.0.1` for local)

**Example:**
```bash
./zappy_gui -p 4242 -h 127.0.0.1
```

3\. Launch an AI Client (`zappy_ai`)
```bash
./zappy_ai -p <port> -n <team_name> -h <server_ip>
```

**Arguments:**

`-p <port>`: server port
`-n <team_name>`: team name to join (must match one given with `-n` in server)
`-h <server_ip>`: IP address of the server

**Example:**

```bash
./zappy_ai -p 4242 -n Alpha -h 127.0.0.1
```

### Technical Architecture

#### Server (`zappy_server` – C)

The server is the core of the game logic. It is responsible for:

-   **Handling TCP client connections** using sockets and `poll()` for I/O multiplexing.
-   **Generating and managing the map**, a toroidal 2D grid with configurable size and resources.
-   **Managing the game clock**: actions have a fixed time cost depending on the `frequency` parameter (used to define time units or ticks).
-   **Parsing and responding to protocol messages**, using a custom ASCII-based message format.
-   Managing:
-   **Players, teams, inventories**
-   **Level-up rituals**
-   **Resource generation and collection**
-   **Broadcast messages**
-   **Ejection mechanics**
-   **Sound events**
-   **Maintaining communication with the GUI client**, sending a stream of real-time game events.

#### Graphical Client (`zappy_gui` – C++)

The graphical client allows observers to visualize the game in real time. It:

-   Displays the **game map**, **players**, **resources**, **player orientations**, **levels**, and **ongoing actions**.
-   Connects to the server via TCP and receives a stream of events such as `pnw`, `ppo`, `plv`, `pin`, `bct`, `seg`, etc.
-   Parses and renders these events as **animations and GUI elements**.
-   Allows users to **track the evolution of teams**, **monitor level-up rituals**, and observe **AI cooperation or conflict**.

#### AI Clients (`zappy_ai`)

Each AI is an autonomous program that:

-   Connects to the server via TCP.
-   Receives **text commands** such as:
-   `look`, `inventory`, `message`, `current level`
-   Sends **actions** such as:
-   `Forward`, `Right`, `Left`, `Take`, `Set`, `Incantation`, `Broadcast`
-   Must **manage food**, **explore the map**, **collect resources**, and **collaborate** with teammates.
-   Often uses a **finite state machine** or **behavior tree** to make decisions.
-   Must handle asynchronous information, **parse protocol strings**, and **react to unexpected events** (e.g., ejection or failed ritual).

### Game Mechanics

#### Resources

-   Types:
-   `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`
-   Usage:
-   Food is needed to survive (each player consumes 1 food every set number of ticks).
-   Other resources are required for **rituals to level up**.

#### Vision System

-   Each AI has a **cone of vision** that expands with their level:
-   Level 1: 1 tile ahead
-   Level 2: 3 tiles in a second row, and so on...
-   The `Look` command returns a **list of visible tiles**, from closest to farthest, including:
-   Players
-   Resources
-   Tile content (encoded in a custom format)

#### Level-Up Rituals (Incantations)

-   Require:
-   A specific **number of players** of the same level on the same tile
-   A specific **combination of resources** on that tile
-   When `Incantation` is issued:
-   All involved players are **locked** for a set duration
-   The ritual **succeeds or fails** depending on whether all conditions remain valid
-   If successful, all involved players **level up simultaneously**
-   If failed, all players are **unlocked with no level change**

### Network Protocol

All communication is done through **text-based messages**.

**Example AI to Server commands**:

Forward\n
Look\n
Take linemate\n
Broadcast need sibur near (2,3)\n
Incantation\n

**Example Server to AI responses**:

ok\n
ko\n
[ player player food ][ food deraumere ][ ... ]\n
Current level: 3\n
Elevation underway\n

**Server to GUI events (examples)**:

pnw #player_id x y orientation level team_name\n
ppo #player_id x y orientation\n
plv #player_id level\n
pin #player_id x y food linemate ...\n
seg team_name\n

### Technical Challenges and Solutions

| Challenge                          | Solution                                                      |
| ---------------------------------- | ------------------------------------------------------------- |
| Supporting many concurrent clients | Used `poll()` for non-blocking multiplexing                   |
| Expanding vision with player level | Implemented a dynamic cone-of-vision algorithm                |
| Toroidal map logic                 | Applied modular arithmetic to wrap around the map             |
| Coordinated rituals                | Locked player state and validated preconditions at runtime    |
| Custom protocol design             | Built a structured message format using text-based events     |
| Smooth GUI updates                 | Parsed and interpreted live server events into animations     |
| Testing AI behavior                | Wrote custom test AIs to simulate and validate key edge cases |

### Personal Contributions (example to adapt)

If you want to contribute to the project, please create a new branch and make a pull request.

-   Implemented the **map system** with toroidal logic and dynamic resource spawning.
-   Developed the **vision algorithm** used in the `Look` command.
-   Worked on **server-side game loop** and **client connection handling** with `poll()`.
-   Designed and implemented the **ritual engine**, checking player states and resource requirements.
-   Contributed to the **graphical client**, including event parsing and player rendering.
-   Developed a **Python AI bot** capable of reaching level 4 autonomously by managing food, exploring, and performing simple rituals.

## Authors
- [@Etienne](https://github.com/Etiennelbre)
- [@Arthur](https://github.com/ZifFiji)
- [@Nolann](https://github.com/MelmanC)
- [@Quentin](https://github.com/quentin-lpr)
- [@Valentin](https://github.com/jeanmiii)
- [@Robin](https://github.com/RobinChassagne)

