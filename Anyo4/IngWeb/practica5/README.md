[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/hTw44pIE)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-7f7980b617ed060a017424585567c406b6ee15c891e84e1186181d67ecf80aa0.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=12935790)
[![Build Status](../../actions/workflows/CI.yml/badge.svg)](../../actions/workflows/CI.yml)

# Web Engineering 2023-2024 / Lab5 Integration and SOA

Please, go to the [GUIDE](docs/GUIDE.md) in order to get the instructions for this assignment.

Some ideas for obtaining a :gift::

- Connect a WebSocket using a bridge
- Store the numbers in a datab ase
- Use complex messages
- Aggregate and split results


# EIP Diagram

![EIP Diagram](EIP-Initial-Config.png)

- SendNumber will only send to the even channel, even if numbers are odd
- The odd number handler and odd number service can't read the same message since the channel is not published: messages are consumed round-robin
- The filter in the odd number handler is not working: odd numbers are resent to the discard channel
