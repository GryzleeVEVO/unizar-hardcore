plugins {
    id("org.springframework.boot") version "3.1.3"
    id("io.gitlab.arturbosch.detekt") version "1.23.1"
    kotlin("jvm") version "1.9.10"
    kotlin("plugin.spring") version "1.9.10"
    kotlin("plugin.jpa") version "1.9.10"
}

group = "es.unizar.webeng"
version = "2023"

val detektVersion by extra("1.23.1")
val kotlinLoggingVersion by extra("5.1.0")

repositories {
    mavenCentral()
}

dependencies {
    implementation(platform(org.springframework.boot.gradle.plugin.SpringBootPlugin.BOM_COORDINATES))
    implementation("org.springframework.boot:spring-boot-starter-websocket")
    implementation("org.jetbrains.kotlin:kotlin-reflect")
    implementation("org.jetbrains.kotlin:kotlin-stdlib-jdk8")
    implementation("io.github.oshai:kotlin-logging-jvm:$kotlinLoggingVersion")

    testImplementation("org.springframework.boot:spring-boot-starter-test")

    detektPlugins("io.gitlab.arturbosch.detekt:detekt-formatting:$detektVersion")
}

tasks.withType<Test> {
    useJUnitPlatform()
    addTestOutputListener { _, outputEvent ->
        logger.lifecycle(outputEvent.message)
    }
}

detekt {
    autoCorrect = true
}