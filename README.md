# Windows Monitoring Agent

Windows Monitoring Agent is a lightweight monitoring agent for Windows systems. Its current version is minimalistic and outputs CPU usage to the console. Future iterations will include data transmission using a custom protocol over sockets.

## Features
- Outputs real-time CPU usage to the console.
- Implements a custom protocol for metric data transmission (work in progress).

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/Valera2312/windows_agent.git
   ```
2. Navigate to the project directory:
   ```bash
   cd windows_agent
   ```
3. Build the project using your preferred development environment or tools.

## Usage
1. Run the application.
2. The console will display the current CPU usage in real-time.

## Roadmap
- [ ] Implement metric data transmission via a custom protocol over sockets.
- [ ] Add support for additional system metrics (e.g., memory usage, disk I/O).
- [ ] Introduce configuration options for customizable monitoring.
- [ ] Provide integration with a central monitoring server.

## Contributing
Contributions are welcome! If you'd like to help improve the project:
1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add some feature"
   ```
4. Push to the branch:
   ```bash
   git push origin feature-name
   ```
5. Open a pull request.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Author
Valera2312

## Feedback
If you have any questions or suggestions, feel free to open an issue or reach out via the repository's issue tracker.
