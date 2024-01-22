@file:Suppress("WildcardImport", "NoWildcardImports", "MagicNumber")

package soa

import org.springframework.beans.factory.annotation.Autowired
import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication
import org.springframework.context.annotation.Bean
import org.springframework.integration.annotation.Gateway
import org.springframework.integration.annotation.MessagingGateway
import org.springframework.integration.annotation.ServiceActivator
import org.springframework.integration.config.EnableIntegration
import org.springframework.integration.dsl.*
import org.springframework.scheduling.annotation.EnableScheduling
import org.springframework.scheduling.annotation.Scheduled
import org.springframework.stereotype.Component
import java.util.*
import java.util.concurrent.atomic.AtomicInteger

@SpringBootApplication
@EnableIntegration
@EnableScheduling
class MyConfiguration {
    @Bean
    fun integerSource(): AtomicInteger = AtomicInteger()

    @Bean
    fun evenChannel(): PublishSubscribeChannelSpec<*> = MessageChannels.publishSubscribe()

    // If not published, both the handler and service will compete to consume the messages.
    @Bean
    fun oddChannel(): PublishSubscribeChannelSpec<*> = MessageChannels.publishSubscribe()

    // Increment handler, now it will send the numbers to a shared number channel
    @Bean
    fun myFlow(integerSource: AtomicInteger): IntegrationFlow = integrationFlow(
        source = { integerSource.getAndIncrement() },
        options = { poller(Pollers.fixedRate(100)) }
    ) {
        route<Int> { "numberChannel" }
    }

    // Consumes numbers from the number channel and resends them to the right channel according to their parity
    @Bean
    fun evenOrOddFlow(): IntegrationFlow = integrationFlow("numberChannel") {
        route<Int> { p -> if (p % 2 == 0) "evenChannel" else "oddChannel" }
    }

    // Receives numbers from the even channel and prints them
    @Bean
    fun evenFlow(): IntegrationFlow = integrationFlow("evenChannel") {
        transform { obj: Int -> "Number $obj" }
        handle { p -> println("Received even: [${p.payload}] of type ${p.payload.javaClass.name}") }
    }

    // Receives numbers from the odd channel and prints them
    @Bean
    fun oddFlow(): IntegrationFlow = integrationFlow("oddChannel") {
        // filter({ p: Int -> p % 2 == 0 }, { discardChannel("discardChannel") })
        transform { obj: Int -> "Number $obj" }
        handle { p -> println("Received odd: [${p.payload}] of type ${p.payload.javaClass.name}") }
    }

    // Unused
    // @Bean
    // fun discarded(): IntegrationFlow = integrationFlow("discardChannel") {
    //    handle { p -> println("Discarded: [${p.payload}] of type ${p.payload.javaClass.name}") }
    // }

    @Autowired
    lateinit var sendNumber: SendNumber

    // Cron job that sends a random number between -100 and 0 each second
    @Scheduled(fixedRate = 1000)
    fun sendNumber() {
        println("Sending number")
        sendNumber.sendNumber(-Random().nextInt(100))
    }
}

@Component
class SomeService {
    // Service will consume messages from the odd channel as a subscriber.
    @ServiceActivator(inputChannel = "oddChannel")
    fun handle(p: Any) {
        println("Service received odd: [$p] of type ${p.javaClass.name}")
    }
}

@MessagingGateway
interface SendNumber {
    // Now requests are sent to the shared number channel
    @Gateway(requestChannel = "numberChannel")
    fun sendNumber(number: Int)
}

fun main() {
    runApplication<MyConfiguration>()
}
