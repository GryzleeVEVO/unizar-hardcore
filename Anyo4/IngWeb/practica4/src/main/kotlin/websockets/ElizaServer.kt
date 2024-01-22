@file:Suppress("NoWildcardImports", "WildcardImport", "SpreadOperator")

package websockets

import io.github.oshai.kotlinlogging.KotlinLogging
import jakarta.websocket.*
import jakarta.websocket.CloseReason.CloseCodes
import jakarta.websocket.server.ServerEndpoint
import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication
import org.springframework.context.annotation.Bean
import org.springframework.context.annotation.Configuration
import org.springframework.stereotype.Component
import org.springframework.web.socket.server.standard.ServerEndpointExporter
import java.util.*

@SpringBootApplication
class Application

fun main(args: Array<String>) {
    runApplication<Application>(*args)
}

@Configuration(proxyBeanMethods = false)
class WebSocketConfig {
    @Bean
    fun serverEndpoint() = ServerEndpointExporter()
}

private val logger = KotlinLogging.logger {}

/**
 * If the websocket connection underlying this [RemoteEndpoint] is busy sending a message when a call is made to send
 * another one, for example if two threads attempt to call a send method concurrently, or if a developer attempts to
 * send a new message while in the middle of sending an existing one, the send method called while the connection
 * is already busy may throw an [IllegalStateException].
 *
 * This method wraps the call to [RemoteEndpoint.Basic.sendText] in a synchronized block to avoid this exception.
 */
fun RemoteEndpoint.Basic.sendTextSafe(message: String) {
    synchronized(this) {
        sendText(message)
    }
}

@ServerEndpoint("/eliza")
@Component
class ElizaEndpoint {

    private val eliza = Eliza()

    /**
     * Successful connection
     *
     * @param session
     */
    @OnOpen
    fun onOpen(session: Session) {
        logger.info { "Server Connected ... Session ${session.id}" }
        with(session.basicRemote) {
            sendTextSafe("The doctor is in.")
            sendTextSafe("What's on your mind?")
            sendTextSafe("---")
        }
    }

    /**
     * Connection closure
     *
     * @param session
     */
    @OnClose
    fun onClose(session: Session, closeReason: CloseReason) {
        logger.info { "Session ${session.id} closed because of $closeReason" }
    }

    /**
     * Message received
     *
     * @param message
     */
    @OnMessage
    fun onMsg(message: String, session: Session) {
        logger.info { "Server Message ... Session ${session.id}" }
        val currentLine = Scanner(message.lowercase(Locale.getDefault()))
        if (currentLine.findInLine("bye") == null) {
            logger.info { "Server received \"${message}\"" }
            runCatching {
                if (session.isOpen) {
                    with(session.basicRemote) {
                        sendTextSafe(eliza.respond(currentLine))
                        sendTextSafe("---")
                    }
                }
            }.onFailure {
                logger.error(it) { "Error while sending message" }
                session.close(CloseReason(CloseCodes.CLOSED_ABNORMALLY, "I'm sorry, I didn't understand that."))
            }
        } else {
            session.close(CloseReason(CloseCodes.NORMAL_CLOSURE, "Alright then, goodbye!"))
        }
    }

    @OnError
    fun onError(session: Session, errorReason: Throwable) {
        logger.error(errorReason) { "Session ${session.id} closed because of ${errorReason.javaClass.name}" }
    }
}
