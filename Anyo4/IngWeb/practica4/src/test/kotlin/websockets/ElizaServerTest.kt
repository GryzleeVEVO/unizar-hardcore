@file:Suppress("NoWildcardImports")

package websockets

import io.github.oshai.kotlinlogging.KotlinLogging
import jakarta.websocket.ClientEndpoint
import jakarta.websocket.ContainerProvider
import jakarta.websocket.OnMessage
import jakarta.websocket.Session
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Assertions.assertTrue
import org.junit.jupiter.api.Test
import org.springframework.boot.test.context.SpringBootTest
import org.springframework.boot.test.context.SpringBootTest.WebEnvironment.RANDOM_PORT
import org.springframework.boot.test.web.server.LocalServerPort
import java.net.URI
import java.util.concurrent.CountDownLatch

private val logger = KotlinLogging.logger {}

@SpringBootTest(webEnvironment = RANDOM_PORT)
class ElizaServerTest {

    @LocalServerPort
    private var port: Int = 0

    @Test
    fun onOpen() {
        logger.info { "This is the test worker" }
        val latch = CountDownLatch(3)
        val list = mutableListOf<String>()

        val client = SimpleClient(list, latch)
        client.connect("ws://localhost:$port/eliza")
        latch.await()
        assertEquals(3, list.size)
        assertEquals("The doctor is in.", list[0])
    }

    @Test
    fun onChat() {
        logger.info { "Test thread" }
        val latch = CountDownLatch(4)
        val list = mutableListOf<String>()

        val client = ComplexClient(list, latch)
        client.connect("ws://localhost:$port/eliza")
        latch.await()
        val size = list.size

        // The list is modified by a client, which runs on a separate thread. Reading its size is not thread-safe,
        // and can add a new message after reading the size. It is better to save the size in a variable with the
        // knowledge that the list is at least a certain size during the assertions.

        assertTrue(size in 4..5)

        // assertEquals() does not take boolean expressions as arguments. A range check is a boolean expression.
        // assertTrue does accept boolean expressions
        // Also, a range is checked because the size assignment is not atomic. This test does not wait for the delimiter
        // to be received before continuing, so it has to account for the case where the delimiter is received before
        // the size variable is assigned.

        assertEquals("The doctor is in.", list[0]) // Opening line
        assertEquals("We were discussing you, not me.", list[3]) // Answer to client question
    }
}

@ClientEndpoint
class SimpleClient(private val list: MutableList<String>, private val latch: CountDownLatch) {

    @OnMessage
    fun onMessage(message: String) {
        logger.info { "Client received: $message" }
        list.add(message)
        latch.countDown()
    }
}

@ClientEndpoint
class ComplexClient(private val list: MutableList<String>, private val latch: CountDownLatch) {

    @OnMessage
    fun onMessage(message: String, session: Session) {
        logger.info { "Client received: $message" }
        list.add(message)
        latch.countDown()

        // If the server connects, send a message to the server
        if (message == "The doctor is in.") {
            // This sentence always returns "We were discussing you, not me."
            // In this case, it is because a "you" was found in the message.
            // Other sentences might return multiple answers
            session.basicRemote.sendText("Hey, how are you")
        }
    }
}

fun Any.connect(uri: String) {
    ContainerProvider.getWebSocketContainer().connectToServer(this, URI(uri))
}
