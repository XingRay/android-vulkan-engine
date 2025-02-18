plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
}

android {
    namespace = "io.github.xingray.vulkandemo"
    compileSdk = 35

    defaultConfig {
        applicationId = "io.github.xingray.vulkandemo"
        minSdk = 28
        targetSdk = 35
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        externalNativeBuild {
            cmake {
                abiFilters.addAll(
                    listOf(
                        //"armeabi-v7a",
                        "arm64-v8a",
                        //"x86",
                        // "x86_64"
                    )
                )

                arguments.addAll(
                    listOf(
                        "-DANDROID_TOOLCHAIN=clang",
//                    "-DANDROID_STL=c++_static",
                        "-DANDROID_STL=c++_shared",
                    )
                )
            }
        }
    }

    externalNativeBuild {
        cmake {
            version = "3.31.1"
            path("src/main/cpp/CMakeLists.txt")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    kotlinOptions {
        jvmTarget = "17"
    }

    ndkVersion = "27.2.12479018"
    buildToolsVersion = "35.0.0"
    buildFeatures {
        viewBinding = false
    }
    dependenciesInfo {
        includeInApk = false
        includeInBundle = false
    }
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)

    implementation(libs.androidx.games.activity)
}