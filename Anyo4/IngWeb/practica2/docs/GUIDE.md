# Lab Assignment Guide 2022-2023

Welcome to the second assignment of the 2022-2023 course.
This guide will walk you through the steps to complete the assignment efficiently.
While this guide is command-line oriented, you are free to use IDEs like **VS Code**, **IntelliJ IDEA**, or **Eclipse**,
which have full support for the tools we are going to use.
Ensure that you have at least **Java 17** installed on your system.

## Getting Started

### Clone the Repository

1. Begin by cloning the assignment repository to your local machine:

    ```bash
    git clone https://github.com/UNIZAR-30246-WebEngineering/lab-2-web-server-<your-github-user>.git
    cd lab-2-web-server-<your-github-user>
    ```

2. Make changes to the files, commit the changes to the history and push the branch up to your repository.

    ```bash
    git push origin main
    ```

### Test the Server

If you wish to test the server locally, utilize the following command:

```bash
./gradlew bootRun
```

## Primary tasks

Your assignment comprises the following tasks:

1. Customize Whitelabel Error Page
2. Add a New Endpoint
3. Enable HTTP/2 and SSL Support

### Customize Whitelabel Error Page

1. Add a template engine in the dependencies.
   For example, you can use  `org.springframework.boot:spring-boot-starter-thymeleaf` for Thymeleaf.
2. Create a custom HTML error page.
3. Save the file in `resources/templates` directory with the name `error.html`

### Add a New Endpoint

1. Create a file named `TimeComponent.kt`.
2. Create inside a DTO for returning the time:

   ```kotlin
   data class TimeDTO(val time: LocalDateTime)
   ```

3. Create an interface that represents the time provider:

    ```kotlin
    interface TimeProvider {
      fun now(): LocalDateTime
    }
    ```

4. Create a service that implements the `TimeProvider` interface:

    ```kotlin
    @Service
    class TimeService: TimeProvider {
      override fun now() = LocalDateTime.now()
    }
    ```

5. Create an extension that transform a `LocalDateTime` into a `TimeDTO` object:

    ```kotlin
    fun LocalDateTime.toDTO() = TimeDTO(time = this)
    ```

6. Create a `RestController` to expose the service:

    ```kotlin
    @RestController 
    class TimeController(val service: TimeProvider) {
      @GetMapping("/time")
      fun time() = service.now().toDTO()
    }
    ```  

### Enable HTTP/2 and SSL Support

1. Use the following command to generate a self-signed certificate:

    ```sh
    openssl req -x509 -out localhost.crt -keyout localhost.key \
    -newkey rsa:2048 -nodes -sha256 \
    -subj '/CN=localhost' -extensions EXT -config <( \
     printf "[dn]\nCN=localhost\n[req]\ndistinguished_name = dn\n[EXT]\nsubjectAltName=DNS:localhost\nkeyUsage=digitalSignature\nextendedKeyUsage=serverAuth")
    ```

    This command generates two files: `localhost.crt` (the public key) and `localhost.key` (the private key).

2. Next, use `openssl` to generate a PKCS truststore file that contains both the certificate and key.
   Remember the export password that you will be asked to enter.
  
    ```sh
    openssl pkcs12 -export -in localhost.crt -inkey localhost.key -name localhost -out localhost.p12
    ```
    
    Move the file `localhost.p12` to `src/main/resources`.
    
    **Note for Windows users:** Unless you use a WSL Linux Distro, you will need to use [OpenSSL for Windows](https://wiki.openssl.org/index.php/Binaries).

3. Create an `application.properties` file in `src/main/resources` with the following content follows:

   ```properties
   # Enable HTTP/2
   server.http2.enabled=true
   # Change the port to 8443
   server.port=8443
   # Enable HTTPS
   server.ssl.key-store-type=PKCS12
   server.ssl.key-store=classpath:localhost.p12
   server.ssl.key-store-password=<the export password>
   server.ssl.key-alias=localhost
   ```

## Manual verification

1. Start the application as usual.

2. To test the generic error file, run the following command in the terminal:

    ```sh
    curl -k -LH "Accept: text/html,*/*;q=0.9" -i https://127.0.0.1:8443/  
    ```
    
    `-k` disables the certificate validation.
    `-LK "Accept: text/html,*/*;q=0.9"` adds the `Accept` header to the request.
    `-i` specify that the output should include the HTTP response headers.
    
    You should see output in the terminal similar to:
    
    ```http
    HTTP/2 404 
    vary: Origin
    vary: Access-Control-Request-Method
    vary: Access-Control-Request-Headers
    content-type: text/html;charset=UTF-8
    content-language: es-ES
    content-length: 134
    date: Sat, 01 Oct 2022 13:59:22 GMT
    ```
    
    The content must be your custom error page.

3. To test HTTP/2 and SSL support, run the following command in the terminal:

   ```sh
   curl -k -LH "Accept: text/html,*/*;q=0.9" -i https://127.0.0.1:8443/time  
   ```

   You should see output in the terminal similar to:

   ```http
   HTTP/2 200 
   content-type: application/json
   content-length: 37
   date: Sat, 01 Oct 2022 13:59:27 GMT
   ```

   Additionally, the entity body must have the following structure:

   ```json
   {
     "time": "2022-10-01T16:33:58.91803" 
   }
   ```

## How to submit

Your work pass if:

- Your `main` branch contains proofs that shows that you have fulfilled the primary tasks.
- The color of your CI workflow is green.

## Detekt and Ktlint are watching you

[Detekt](https://detekt.dev/) helps you write cleaner Kotlin code, so you can focus on what matters the most building amazing software.
It is configured to run before compiling the code.

- By default, the standard rule set without any ignore list is executed on sources files located in `src/main/java`, `src/test/java`, `src/main/kotlin` and `src/test/kotlin`.
- Reports are automatically generated in xml, html, txt, md, and sarif format and can be found in build/reports/detekt/detekt.[xml|html|txt|md|sarif] respectively.
- Also, the [Ktlint](https://ktlint.github.io/) plugin has been enabled. It will automatically format your code (if it can) respectively to the [Kotlin Coding Conventions](https://kotlinlang.org/docs/coding-conventions.html#source-code-organization). If the source code is changed the build will fail.
