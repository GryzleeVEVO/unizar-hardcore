package es.unizar.webeng.lab2.component

import org.springframework.stereotype.Service
import java.time.LocalDateTime

data class TimeDTO(val time: LocalDateTime)

interface TimeProvider {
    fun now(): LocalDateTime
}

@Service
class TimeService : TimeProvider {
    override fun now(): LocalDateTime = LocalDateTime.now()
}
fun LocalDateTime.toDTO() = TimeDTO(time = this)
