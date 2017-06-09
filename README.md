## What is JabberHive?
JabberHive is a modular ChatBot system. All "modules" are in fact separate
programs linked together using the JabberHive Protocol. Please refer to the
protocol for more information.

## Component Description
* Filter for a JabberHive network.
* Randomly transforms ?RLR queries into ?RL ones.
* The transformation chance is a run-time parameter.

## JabberHive Protocol Compatibility
* **Protocol Version(s):** 1.
* **Inbound Connections:** Multiple.
* **Outbound Connections:** Multiple.
* **Pipelining:** No.
* **Behavior:** Filter.

## Notes
* Does not correctly reply to Pipelining & Protocol Version requests.

## Dependencies
- POSIX compliant OS.
- C compiler (with C99 support).
- CMake.
