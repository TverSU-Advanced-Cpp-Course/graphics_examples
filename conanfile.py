from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class GraphicsExamplesConan(ConanFile):
    name = "graphics_examples"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    default_options = {
        "glad/*:gl_profile": "core",
        "glad/*:gl_version": "3.3",
    }

    def requirements(self):
        self.requires("glfw/[>=3.4 <4]")
        self.requires("glad/0.1.36")
        self.requires("glm/1.0.1")
        self.requires("raylib/[>=5.0 <6]")
        self.requires("stb/cci.20240531")

    def build_requirements(self):
        self.tool_requires("ninja/[>=1.11]")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
