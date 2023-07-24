# Webserv - An HTTP Server Implementation

## Table of Contents

- [Introduction](#introduction)
- [Requirements](#requirements)
- [Usage](#usage)
- [Configuration File](#configuration-file)
- [Supported Features](#supported-features)
- [Bonus Features](#bonus-features)
- [Testing](#testing)
- [Compatibility](#compatibility)
- [Contributing](#contributing)

## Introduction

Webserv is a custom HTTP server implementation that adheres to the HTTP 1.1 protocol. It aims to provide a non-blocking and efficient server capable of serving static websites and handling various HTTP methods like GET, POST, and DELETE. The server is designed to be highly resilient, ensuring it stays available under stress tests.

## Requirements

- C++ Compiler (C++11 or later)
- MacOS (for specific instructions, see section III.2)
- [Test Environment](https://link-to-test-environment.com) (optional but recommended)

## Usage

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/webserv.git
   cd webserv
2. **Run the following command**:

   ```bash
   make

3. **Run the server with the default configuration file**:

   ```bash
   ./webserv path/to/your/config_file

## Configuration File

The configuration file for Webserv is inspired by the 'server' part of the NGINX configuration file. It allows you to set up multiple servers with different configurations. Here are some of the options you can specify in the configuration file:

- Choose the port and host for each server.
- Define server names and set default servers for a host:port.
- Configure default error pages.
- Limit client body size.
- Define routes with various rules such as HTTP methods, redirection, directory, file, directory listing, default files, CGI execution, etc.

Please refer to the provided sample configuration files for more details.

## Supported Features

- Fully static website serving.
- GET, POST, and DELETE methods.
- Non-blocking I/O using poll() or equivalent.
- Support for multiple ports and hosts.
- Stress-tested for high availability.
- Default error pages.
- Ability to accept uploaded files from clients.

## Bonus Features

Webserv includes the ability to handle multiple CGI scripts, providing you with the flexibility to execute various CGI programs based on their file extensions. You can configure the extensions and corresponding binary paths in the server's configuration file.

## Configuration Example

To add custom CGI scripts, modify your configuration file as follows:

```ini
# server_config.conf

server {
    port 80;
    host localhost;

    # Define the CGI binaries and their corresponding extensions
    cgi_bin                 py /Users/aessabir/Desktop/webserv/cgi/python-cgi;
    cgi_bin                 php /Users/aessabir/Desktop/webserv/cgi/php-cgi;

    # Additional server configuration...
}
```
In this example, we've defined two CGI extensions, `.py` and `.php`, along with their respective binary paths. This allows the server to execute Python scripts with the `/usr/bin/python3` binary and PHP scripts with the `/usr/bin/php-cgi` binary.

## Default CGI Scripts

If you do not specify custom configurations for a particular CGI extension, Webserv will use the following default configurations:

- For `.py` files, the server will execute them using the `/usr/bin/python3` binary.
- For `.php` files, the server will execute them using the `/usr/bin/php-cgi` binary.

## Using CGI in Routes

To enable CGI execution for specific routes, you need to set up a route in the configuration file and associate it with the desired CGI extension.

## Using CGI in Location

To enable CGI execution for specific Location, you need to set up a Location in the configuration file and associate it with the desired CGI extension.

```ini
# server_config.conf

server {
    port 80;
    host localhost;

    Location /scripts {
        # Enable CGI for the route by specifying the ".py" extension
        cgi_bin                 py /path_to/python-cgi;
    }

    # Additional server configuration...
}
```
With this configuration, any request to the "/scripts" Location with a ".py" file extension will be handled as a CGI script and executed using the specified Python binary.

**Please ensure that the CGI scripts you intend to execute have the necessary permissions to be executed by the server.**

## Contributing

We welcome contributions to the project. If you find any issues or have suggestions for improvement, please do the following:

1. Check the [Issues](https://github.com/ayoubessabiry/webserv/issues) section to see if the issue or suggestion has already been raised.

2. If it's a new issue or suggestion, [create a new issue](https://github.com/ayoubessabiry/webserv/issues/new) to describe it in detail. Include steps to reproduce the problem or a clear description of the proposed improvement.

3. If you'd like to contribute code to address an issue or implement a feature, you can [fork the repository](https://github.com/ayoubessabiry/webserv/fork) and create a new branch for your changes.



