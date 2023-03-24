# Webserv

## Description

Webserv is a web server implementation written in C++. It can handle multiple clients simultaneously, and supports GET, POST, and DELETE methods. It also supports serving static files, running CGI scripts, and displaying directory contents when no index file is present.

## Installation

Clone the repository:

```bash
git clone https://github.com/agatocherry/webserv.git
```

Compile the server:

```bash
make
```

## Usage

To start the server with the default configuration file, run:

```bash
./webserv config/default.conf
```

To start the server with a custom configuration file, replace config/default.conf with the path to your configuration file.

To send a request to the server, use a web browser or a command-line tool such as curl. For example, to request the root of the server, run:

```bash
curl http://localhost:8082/
```

## Documentation

See the [documentation.md](./documentation.md) file for documentation related to this project.

## Credits

This project was developed by [@agatocherry](https://github.com/agatocherry), [@shell02](https://github.com/shell02) and [@Britney94](https://github.com/Britney94) for 42 school.