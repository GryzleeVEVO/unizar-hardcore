# Lab Assignment Guide 2023-2024

Welcome to the third assignment of the 2023-2024 course.
This guide will walk you through the steps to complete the assignment efficiently.
While this guide is command-line oriented, you are free to use IDEs like **VS Code**, **IntelliJ IDEA**, or **Eclipse**,
which have full support for the tools we are going to use.
Ensure that you have at least **Java 17** installed on your system.

The Web API of this laboratory is a straightforward CRUD application designed to manage employee data. It facilitates Create, Read, Update, and Delete operations for employees.

The choice of H2 as the database, specifically an in-memory database, is appropriate for this application. In-memory databases are useful, especially for scenarios where persistent storage is not a primary concern.

The persistence layer is effectively managed through a Spring Repository, named *EmployeeRepository*. Utilizing Spring Data JPA repositories is a good practice as it simplifies data access and provides a convenient way to perform CRUD operations on the database.

Regarding testing, we plan to mock the repository for verification purposes. Mocking the repository allows for isolated testing of the controller's functionality without interacting with the actual database. This approach is crucial for verifying that the controller has a correct implementation of the API's HTTP methods.

## Getting Started

### Clone the Repository

1. Begin by cloning the assignment repository to your local machine:

    ```bash
    git clone https://github.com/UNIZAR-30246-WebEngineering/lab-3-web-api-<your-github-user>.git
    cd lab-3-web-api-<your-github-user>
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

## Primary task

- Complete tests of `tests.kt`

## Steps required

There are eight tests covering the following aspects:

- Safe methods (`GET`)
- Unsafe methods (`PUT`, `DELETE`, `POST`)
- Idempotent methods (`GET`, `PUT`, `DELETE`)
- Non-idempotent method (`POST`)

To complete the test scenarios, utilize the code blocks provided below for:

- Setting up the calls to the repository
- Writing the expect block for an HTTP response
- Verifying the calls made to the repository

---

### Setting up the calls to the repository

Searching for an employee with `id` 1.

```kotlin
every {
    employeeRepository.findById(1)
} answers {
    Optional.of(Employee("Mary", "Manager", 1))
}
```

Searching for an employee with `id` 1 that is not found.

```kotlin
every {
    employeeRepository.findById(1)
} answers {
    Optional.empty()
}
```

Searching for an employee with `id` 1 that is not found and in a second call is found.

```kotlin
every {
    employeeRepository.findById(1)
} answers {
    Optional.empty()
} andThenAnswer {
    Optional.of(Employee("Tom", "Manager", 1))
}
```

Searching for an employee with `id` 1 that is found and in a second call is not found.

```kotlin
every {
    employeeRepository.findById(1)
} answers {
    Optional.of(Employee("Tom", "Manager", 1))
} andThenAnswer {
    Optional.empty()
}
```

Saving employees with null `id` and assigning a new `id`.

```kotlin
val employee = slot<Employee>()
every {
    employeeRepository.save(capture(employee))
} answers {
    assertThat(employee.captured.id).isNull()
    employee.captured.copy(id = 1)
}
```

Saving employees with null `id` and assigning a new `id`.

```kotlin
val employee = slot<Employee>()
every {
    employeeRepository.save(capture(employee))
} answers {
    assertThat(employee.captured.id).isNull()
    employee.captured.copy(id = 1)
} andThenAnswer {
    assertThat(employee.captured.id).isNull()
    employee.captured.copy(id = 2)
}
```

Saving employees with non-null `id`.

```kotlin
val employee = slot<Employee>()
every {
    employeeRepository.save(capture(employee))
} answers {
    assertThat(employee.captured.id).isNotNull()
    employee.captured.copy()
}
```

Deleting employees with `id` equal to 1.

```kotlin
justRun {
    employeeRepository.deleteById(1)
}
```

### Writing the expect block for an HTTP response

A successful response with a body.

```kotlin
status { isOk() }
content {
    contentType(MediaType.APPLICATION_JSON)
    json(MANAGER_RESPONSE_BODY("Mary", 1))
}
```

A created response with a body.

```kotlin
status { isCreated() }
header { string("Location", "http://localhost/employees/2") }
content {
    contentType(MediaType.APPLICATION_JSON)
    json(MANAGER_RESPONSE_BODY("Mary", 2))
}
```

A response with a body that does not imply the creation of a resource.

```kotlin
status { isOk() }
header { string("Content-Location", "http://localhost/employees/1") }
content {
    contentType(MediaType.APPLICATION_JSON)
    json(MANAGER_RESPONSE_BODY("Tom", 1))
}
```

A response for a not found resource.

```kotlin
status { isNotFound() }
```

### Verifying the calls made to the repository

`findById` is called once.

```kotlin
verify(exactly = 1) {
    employeeRepository.findById(1)
}
```

`findById` is called twice.

```kotlin
verify(exactly = 2) {
    employeeRepository.findById(1)
}
```

`findById` and `save` are called twice.

```kotlin
verify(exactly = 2) {
    employeeRepository.findById(1)
    employeeRepository.save(Employee("Tom", "Manager", 1))
}
```

`save` is called once.

```kotlin
verify(exactly = 1) {
    employeeRepository.save(Employee("Mary", "Manager", null))
}
```

`save` is called once.

```kotlin
verify(exactly = 1) {
    employeeRepository.save(Employee("Tom", "Manager", 1))
}
```

`save` is called twice.

```kotlin
verify(exactly = 2) {
    employeeRepository.save(Employee("Mary", "Manager", null))
}
```

`deleteById` is called once.

```kotlin
verify(exactly = 1) {
    employeeRepository.deleteById(1)
}
```

`findById` is called before `save`.

```kotlin
verifyOrder {
    employeeRepository.findById(1)
    employeeRepository.save(Employee("Tom", "Manager", 1))
}
```

The sequence is `findById`, `save`, `findById`, `save`.

```kotlin
verifyOrder {
    employeeRepository.findById(1)
    employeeRepository.save(Employee("Tom", "Manager", 1))
    employeeRepository.findById(1)
    employeeRepository.save(Employee("Tom", "Manager", 1))
}
```

`findById` is called before `deleteById`.

```kotlin
verifyOrder {
    employeeRepository.findById(1)
    employeeRepository.deleteById(1)
}
```

The sequence is `findById`, `deleteById`, `findById`.

```kotlin
verifyOrder {
    employeeRepository.findById(1)
    employeeRepository.deleteById(1)
    employeeRepository.findById(1)
}
```
}

## How to submit

Push your code to GitHub.

Your work will pass if:

- `tests.kt` validates that the controller implementation is OK.
- The color of your CI workflow is green.

## Detekt and Ktlint are watching you

[Detekt](https://detekt.dev/) helps you write cleaner Kotlin code, so you can focus on what matters the most building amazing software.
It is configured to run before compiling the code.

- By default, the standard rule set without any ignore list is executed on sources files located in `src/main/java`, `src/test/java`, `src/main/kotlin` and `src/test/kotlin`.
- Reports are automatically generated in xml, html, txt, md, and sarif format and can be found in build/reports/detekt/detekt.[xml|html|txt|md|sarif] respectively.
- Also, the [Ktlint](https://ktlint.github.io/) plugin has been enabled. It will automatically format your code (if it can) respectively to the [Kotlin Coding Conventions](https://kotlinlang.org/docs/coding-conventions.html#source-code-organization). If the source code is changed the build will fail.