package es.unizar.webeng.hello.controller

import org.hamcrest.CoreMatchers.*
import org.junit.jupiter.api.Test
import org.springframework.beans.factory.annotation.Autowired
import org.springframework.beans.factory.annotation.Value
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest
import org.springframework.test.web.servlet.MockMvc
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*
import org.springframework.test.web.servlet.result.MockMvcResultHandlers.*
import org.springframework.test.web.servlet.result.MockMvcResultMatchers.*

/**
 * Test class for the Hello World MVC model
 *
 * @constructor Create empty Hello controller MVC tests
 */
@WebMvcTest(HelloController::class) class HelloControllerMVCTests {
    @Value("\${app.message}") private lateinit var message: String

    @Autowired private lateinit var mockMvc: MockMvc

    /**
     * Tests if the MVC is created correctly and the "message" field is set
     * correctly.
     */
    @Test fun testMessage() {
        mockMvc.perform(get("/"))
            .andDo(print())
            .andExpect(status().isOk)
            .andExpect(
                model().attribute(
                    "message", equalTo(message)
                )
            )
    }
}

