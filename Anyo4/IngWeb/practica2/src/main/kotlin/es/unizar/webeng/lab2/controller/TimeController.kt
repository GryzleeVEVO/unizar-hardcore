package es.unizar.webeng.lab2.controller

import es.unizar.webeng.lab2.component.TimeProvider
import es.unizar.webeng.lab2.component.toDTO
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.RestController

@RestController
class TimeController(val service: TimeProvider) {

    @GetMapping("/time")
    fun time() = service.now().toDTO()
}
