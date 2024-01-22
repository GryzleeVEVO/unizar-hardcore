# Lab Assignment Guide 2023-2024

Welcome to the first assignment of the 2023-2024 course. 
This guide will walk you through the steps to complete the assignment efficiently.
While this guide is command-line oriented, you are free to use IDEs like **VS Code**, **IntelliJ IDEA**, or **Eclipse**,
which have full support for the tools we are going to use. 
Ensure that you have at least **Java 17** installed on your system.

## System requirements

This application leverages cutting-edge technologies to deliver a robust and versatile user experience:

1. **Programming Language**: The application is written in [Kotlin 1.9.10](https://kotlinlang.org/), a versatile,
   open-source, statically-typed language. Kotlin is renowned for its adaptability and is commonly used for
   Android mobile app development. Beyond that, it finds application in server-side development, making it a versatile
   choice.

2. **Build System**: The application utilizes [Gradle 8.3](https://gradle.org/) as build system. Gradle is renowned for
   its flexibility in automating the software building process. This build automation tool streamlines tasks such as
   compiling, linking, and packaging code, ensuring consistency and reliability throughout development.

3. **Framework**: The application employs [Spring Boot 3.1.3](https://docs.spring.io/spring-boot/docs/3.1.3/reference/htmlsingle/)
   as a framework. This technology requires Java 17 and is fully compatible up to and including Java 20.
   Spring Boot simplifies the creation of production-grade [Spring-based applications](https://spring.io/).
   It adopts a highly opinionated approach to the Spring platform and third-party libraries,
   enabling developers to initiate projects with minimal hassle.

## Objective

The primary objective is to document and enhance existing code without breaking it. 
Each student should **add a minimum of 100 lines of documentation and code improvements** to both 
the source code files and [description.md](../description.md). 
Your documentation should follow [Kotlin best practices](https://kotlinlang.org/docs/kotlin-doc.html) for Kotlin files 
and use [GitHub Markdown](https://guides.github.com/features/mastering-markdown/) in `.md` files.

The topics expected to be covered in your documentation include:

- How to build the code.
- How to test the code.
- How to deploy the code to a server.
- Technologies used in the code.
- How these technologies work.
- Explanation of specific pieces of code.
- The purpose of specific Java annotations.
- ...

Remember that the documentation must be in English (at least B1 level). 

Outstanding contributions will receive a cumulative bonus of 5% in your personal score during the project evaluation. 
Outstanding contributions are defined as those that can be used as reference documentation, 
contain new working and tested code (following [TDD](https://en.wikipedia.org/wiki/Test-driven_development) best practices), 
or go beyond the standard use of _git_, _Gradle_, or _GitHub Actions_.

## Assignment Submission Deadline

If you do not submit anything before October 10th,
you will incur a penalty of 20% in your personal score for the evaluation of the _URLShortener_ project. 
This penalty will be effective only if you have not attempted to submit something of value.

If you have any doubts about the procedure, 
please post an [Issue](https://github.com/UNIZAR-30246-WebEngineering/lab1-git-race/issues).

## Getting Started

### Set up git

1. Start by installing [git](https://git-scm.com/), which is essential for this assignment.

2. Sign up for an education account on [GitHub.com](https://github.com) if you haven't already.
   Use your real name and your university email address.

3. [Configure git](https://docs.github.com/en/get-started/quickstart/set-up-git#setting-up-git)
   on your local machine by running the following commands in your console:

   ```bash
   git config --global user.name "Your Real Name"
   git config --global user.email "your_nip@unizar.es"
   ```

4. [Authenticate with GitHub](https://docs.github.com/en/get-started/quickstart/set-up-git), either via _HTTPS_ (recommended) or _SSH_.

### Clone locally this repository

3. Clone follow these steps:

   ```bash
   git clone https://github.com/your-github-username/lab1-git-race-your-github-username
   cd lab1-git-race-your-github-username
   ```

4. Test your local repository. Open the `lab1-git-race-your-github-username` folder in your terminal and run the following command:

   ```
   ./gradlew check
   ```

### Complete the documentation

Review the code and add relevant and correct information to your documentation. 
Once you've updated it locally, ensure that your code works with `./gradlew check`. 
If it does, push your changes with `git push`.
