[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/lORGAMnM)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-7f7980b617ed060a017424585567c406b6ee15c891e84e1186181d67ecf80aa0.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=12714095)
[![Build Status](../../actions/workflows/CI.yml/badge.svg)](../../actions/workflows/CI.yml)

# Web Engineering 2023-2024 / Lab4 WebSocket

Please, go to the [GUIDE](docs/GUIDE.md) in order to get the instructions for this assignment.

Some ideas for obtaining a :gift: if you are the first that:

- **STOMP!**: Support *STOMP* in the server side and create a small client that uses it.
- **Sock from WebSocket**: Support *SockJS* in the server side and show that polling can be used as transport instead of *WebSocket* when needed ([additional info](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#websocket-fallback-sockjs-client)).
- **Turn sides**: Turn the DOCTOR into a client of the server, so, the server acts only as message broker.
- **Relay**: Use the server as a relay server to connect to an external message broker (e.g., RabbitMQ, [additional info](https://docs.spring.io/spring-framework/docs/current/reference/html/web.html#websocket-stomp-handle-broker-relay))

Note: unless the goal specifies o disallows a specific framework you are free to replace the framework used in the original implementation with a different framework.
