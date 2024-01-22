# Lab 1: Git Race

## Description

The project is a simple Spring Boot application that shows a Hello World
page with a greeting message.

## Technologies

The project uses the following technologies:

- **Kotlin** (1.9.10): A modern programming language created by JetBrains.
  Kotlin runs on the JVM. Compared to Java, it has a more concise syntax,
  type inference and is completely interoperable with existing Java code. It's
  the default language for Android app development.
- **Gradle** (8.3): A build automation tool for Java, designed to replace
  older options such as Ant or Maven.
- **Spring Boot** (3.1.3): A popular framework for creating web based
  applications with Java.
- **Thymeleaf** (3.1): A server-side templating engine for Java.
- **Bootstrap** (5.1.0): A popular CSS library that simplifies UI styling.

## Structure

The project has the following structure:

- src
    - main: Contains the source files of the application
        - kotlin: Source code for the app
            - controller: HTTP request handlers for the app.
                - **HelloController.kt**
            - **HelloWorld.kt**: Entry point for the app.
        - resources
            - templates: HTML files with Thymeleaf placeholder fields.
                - **welcome.html**: Main page.
            - public:
                - assets: Images, graphics, etc...
            - static (not used): Static web content
            - **application.properties**: Stores properties that can be
              retrieved
              in the source code.
    - test: Contains test files for the app
        - **IntegrationTest.kt**: Tests if the web service is run correctly
        - controller:
            - **HelloControllerUnitTests.kt**: Checks if a standalone
              HelloController
              class is created correctly and if responds correctly.
            - **HelloControllerMVCTests.kt**: Checks if the controller, model
              and
              view are created correctly without starting a complete web app.
- **build.gradle.kts**: Defines plugins, dependencies, JVM version, etc...
- **description.md**: THIS FILE
- **gradlew**: Script for running Gradle tasks. It downloads a standalone 
  Gradle jar file if necessary.

## Building the project

The project can be bundled into a jar file using `gradlew`:

```shell
./gradlew bootJar
```

A local development session can be launched with:

```shell
./gradlew bootrun
```
The service will be launched on `localhost:8080`

## Testing the project

The test module can be run using:

```shell
./gradlew test
```

Otherwise, you can check additionally for compilation errors using:

```shell
./gradlew check
```

## Deploying the project

A Docker image can be built using:

```shell
./gradlew bootBuildImage
```
Downloading and building the image may take a while. Once created, it can be 
launched using:

```shell
docker run --name lab1 lab1-git-race:0.0.1-SNAPSHOT 
```

A server will be started on port 8080. To get the IP address of the container:

```shell
docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' lab1
```

## Additions

A simple controller and view have been added for the route /click. Additionally, the 
main page has a button that, when clicked, navigates to this new page.  