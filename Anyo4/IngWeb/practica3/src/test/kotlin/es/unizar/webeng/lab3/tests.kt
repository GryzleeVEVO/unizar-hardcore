@file:Suppress("WildcardImport", "NoWildcardImports", "Filename")

package es.unizar.webeng.lab3

import com.ninjasquad.springmockk.MockkBean
import io.mockk.*
import org.assertj.core.api.Assertions.assertThat
import org.junit.jupiter.api.Test
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc
import org.springframework.boot.test.context.SpringBootTest
import org.springframework.boot.test.context.SpringBootTest.WebEnvironment
import org.springframework.http.MediaType
import org.springframework.test.web.servlet.*
import java.util.*

private val MANAGER_REQUEST_BODY = { name: String ->
    """
    { 
        "role": "Manager", 
        "name": "$name" 
    }
    """
}

private val MANAGER_RESPONSE_BODY = { name: String, id: Int ->
    """
    { 
       "name" : "$name",
       "role" : "Manager",
       "id" : $id
    }
    """
}

@SpringBootTest(webEnvironment = WebEnvironment.MOCK)
@AutoConfigureMockMvc
class SafeMethodsTests {

    @Autowired
    private lateinit var mvc: MockMvc

    @MockkBean
    @Suppress("UnusedPrivateProperty")
    private lateinit var employeeRepository: EmployeeRepository

    @Test
    fun `POST is not safe`() {
        val employee = slot<Employee>()

        // findById should fail once, then succeed
        every {
            employeeRepository.findById(1)
        } answers {
            Optional.empty()
        } andThenAnswer {
            Optional.of(Employee("Mary", "Manager", 1))
        }

        // save should add Mary, 1
        every {
            employeeRepository.save(capture(employee))
        } answers {
            assertThat(employee.captured.id).isNull()
            employee.captured.copy(id = 1)
        }

        // First GET fails
        mvc.get("/employees/1").andExpect { status { isNotFound() } }

        // POST adds Mary to the repository
        mvc.post("/employees") {
            contentType = MediaType.APPLICATION_JSON
            content = MANAGER_REQUEST_BODY("Mary")
            accept = MediaType.APPLICATION_JSON
        }.andExpect {
            status { isCreated() }
            header { string("Location", "http://localhost/employees/1") }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 1))
            }
        }

        // Now GET should return Mary, 1
        mvc.get("/employees/1").andExpect {
            status { isOk() }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 1))
            }
        }

        verifyOrder {
            employeeRepository.findById(1)
            employeeRepository.save(Employee("Mary", "Manager", null))
            employeeRepository.findById(1)
        }
    }

    @Test
    fun `GET is safe`() {
        // findByID always retrieves Mary, 1
        every {
            employeeRepository.findById(1)
        } answers {
            Optional.of(Employee("Mary", "Manager", 1))
        }

        // First GET
        mvc.get("/employees/1").andExpect {
            status { isOk() }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 1))
            }
        }

        // Second GET
        mvc.get("/employees/1").andExpect {
            status { isOk() }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 1))
            }
        }

        verify(exactly = 2) {
            employeeRepository.findById(1)
        }
    }

    @Test
    fun `PUT is not safe`() {
        val employee = slot<Employee>()

        // findById should fail once, then succeed
        every {
            employeeRepository.findById(1)
        } answers {
            Optional.empty()
        } andThenAnswer {
            Optional.of(Employee("Tom", "Manager", 1))
        }

        // save should add Mary, 1
        every {
            employeeRepository.save(capture(employee))
        } answers {
            assertThat(employee.captured.id).isEqualTo(1)
            employee.captured.copy()
        }

        // PUT method alters ID 1 (creates it if not found). PUT also calls findByID once
        mvc.put("/employees/1") {
            contentType = MediaType.APPLICATION_JSON
            content = MANAGER_REQUEST_BODY("Tom")
            accept = MediaType.APPLICATION_JSON
        }.andExpect {
            status { isCreated() }
            header { string("Content-Location", "http://localhost/employees/1") }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Tom", 1))
            }
        }

        // Second GET retrieves Tom
        mvc.get("/employees/1").andExpect {
            status { isOk() }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Tom", 1))
            }
        }

        // Tom is added once and then found
        verifyOrder {
            employeeRepository.findById(1)
            employeeRepository.save(Employee("Tom", "Manager", 1))
            employeeRepository.findById(1)
        }
    }

    @Test
    fun `DELETE is not safe`() {
        // First GET succeeds, next one fails
        every {
            employeeRepository.findById(1)
        } answers {
            Optional.of(Employee("Mary", "Manager", 1))
        } andThenAnswer {
            Optional.empty()
        }

        justRun {
            employeeRepository.deleteById(1)
        }

        // Mary is deleted. DELETE uses findByID
        mvc.delete("/employees/1").andExpect {
            status { isNoContent() }
        }

        // Second GET fails
        mvc.get("/employees/1").andExpect { status { isNotFound() } }

        verifyOrder {
            employeeRepository.findById(1)
            employeeRepository.deleteById(1)
            employeeRepository.findById(1)
        }
    }
}

@SpringBootTest(webEnvironment = WebEnvironment.MOCK)
@AutoConfigureMockMvc
class IdempotentMethodsTests {

    @Autowired
    private lateinit var mvc: MockMvc

    @MockkBean
    @Suppress("UnusedPrivateProperty")
    private lateinit var employeeRepository: EmployeeRepository

    @Test
    fun `POST is not idempotent`() {
        val employee = slot<Employee>()

        // First save should add Mary, 1, second Mary, 2
        every {
            employeeRepository.save(capture(employee))
        } answers {
            assertThat(employee.captured.id).isNull()
            employee.captured.copy(id = 1)
        } andThenAnswer {
            assertThat(employee.captured.id).isNull()
            employee.captured.copy(id = 2)
        }

        // POST adds Mary, 1 to the repository
        mvc.post("/employees") {
            contentType = MediaType.APPLICATION_JSON
            content = MANAGER_REQUEST_BODY("Mary")
            accept = MediaType.APPLICATION_JSON
        }.andExpect {
            status { isCreated() }
            header { string("Location", "http://localhost/employees/1") }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 1))
            }
        }

        // Second POST adds Mary, 2 to repository
        mvc.post("/employees") {
            contentType = MediaType.APPLICATION_JSON
            content = MANAGER_REQUEST_BODY("Mary")
            accept = MediaType.APPLICATION_JSON
        }.andExpect {
            status { isCreated() }
            header { string("Location", "http://localhost/employees/2") }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 2))
            }
        }

        // A null-id Mary is saved twice
        verify(exactly = 2) {
            employeeRepository.save(Employee("Mary", "Manager", null))
        }
    }

    @Test
    fun `GET is idempotent`() {
        // NOTE: Since GET is already safe, it is also idempotent. The same test applies
        // findByID always retrieves Mary, 1
        every {
            employeeRepository.findById(1)
        } answers {
            Optional.of(Employee("Mary", "Manager", 1))
        }

        // First GET
        mvc.get("/employees/1").andExpect {
            status { isOk() }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 1))
            }
        }

        // Second GET
        mvc.get("/employees/1").andExpect {
            status { isOk() }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Mary", 1))
            }
        }

        // findById should be called twice
        verify(exactly = 2) {
            employeeRepository.findById(1)
        }
    }

    @Test
    fun `PUT is idempotent`() {
        val employee = slot<Employee>()

        // Tom should always be added as ID 1
        every {
            employeeRepository.save(capture(employee))
        } answers {
            assertThat(employee.captured.id).isEqualTo(1)
            employee.captured.copy()
        }

        // First findByID fails then succeeds
        every {
            employeeRepository.findById(1)
        } answers {
            Optional.empty()
        } andThenAnswer {
            Optional.of(Employee("Mary", "Manager", 1))
        }

        // First PUT adds Tom with ID 1
        mvc.put("/employees/1") {
            contentType = MediaType.APPLICATION_JSON
            content = MANAGER_REQUEST_BODY("Tom")
            accept = MediaType.APPLICATION_JSON
        }.andExpect {
            status { isCreated() }
            header { string("Content-Location", "http://localhost/employees/1") }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Tom", 1))
            }
        }

        // Second PUT also adds Tom with ID 1, without changes
        mvc.put("/employees/1") {
            contentType = MediaType.APPLICATION_JSON
            content = MANAGER_REQUEST_BODY("Tom")
            accept = MediaType.APPLICATION_JSON
        }.andExpect {
            status { isOk() }
            header { string("Content-Location", "http://localhost/employees/1") }
            content {
                contentType(MediaType.APPLICATION_JSON)
                json(MANAGER_RESPONSE_BODY("Tom", 1))
            }
        }

        verifyOrder {
            employeeRepository.findById(1)
            employeeRepository.save(Employee("Tom", "Manager", 1))
            employeeRepository.findById(1)
            employeeRepository.save(Employee("Tom", "Manager", 1))
        }
    }

    @Test
    fun `DELETE is idempotent`() {
        // First GET succeeds, next one fails
        every {
            employeeRepository.findById(1)
        } answers {
            Optional.of(Employee("Mary", "Manager", 1))
        } andThenAnswer {
            Optional.empty()
        }

        justRun {
            employeeRepository.deleteById(1)
        }

        // Mary is deleted. DELETE uses findByID
        mvc.delete("/employees/1").andExpect {
            status { isNoContent() }
        }

        // Tries to delete again, but is not found.
        mvc.delete("/employees/1").andExpect {
            status { isNotFound() }
        }

        verifyOrder {
            employeeRepository.findById(1)
            employeeRepository.deleteById(1)
            employeeRepository.findById(1) // Second DELETE does not call repository.delete
        }
    }
}
