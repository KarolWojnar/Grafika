#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ustawienia
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// kod Ÿród³owy shadera wierzcho³ków
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// kod Ÿród³owy shadera fragmentów
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: inicjalizacja i konfiguracja
    // ----------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: tworzenie okna
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hourglass", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Nie uda³o siê utworzyæ okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: wczytanie wskaŸników do funkcji OpenGL
    // -------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie uda³o siê zainicjowaæ GLAD" << std::endl;
        return -1;
    }


    // kompilacja i zlinkowanie programu shaderów
    // -----------------------------------------
    // shader wierzcho³ków
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // sprawdzenie b³êdów kompilacji shadera
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "B£¥D::SHADER::WIERZCHO£EK::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // shader fragmentów
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // sprawdzenie b³êdów kompilacji shadera
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "B£¥D::SHADER::FRAGMENT::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // zlinkowanie shaderów
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // sprawdzenie b³êdów linkowania
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "B£¥D::SHADER::PROGRAM::LINKOWANIE_NIEUDANE\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // konfiguracja danych wierzcho³ków
    // --------------------------------
    float vertices1[] = {
        -0.4f, -0.6f, 0.0f, // lewy dolny punkt
         0.4f, -0.6f, 0.0f, // prawy dolny punkt
         0.0f,  0.0f, 0.0f  // górny punkt
    };

    float vertices2[] = {
        -0.4f, 0.6f, 0.0f, // lewy górny punkt
         0.4f, 0.6f, 0.0f, // prawy górny punkt
         0.0f, 0.0f, 0.0f  // górny punkt
    };

    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    // powi¹zanie Vertex Array Object (VAO) jako pierwszego, nastêpnie powi¹zanie i skonfigurowanie bufora wierzcho³ków (VBO) i atrybutów wierzcho³ków
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // odbindowanie VBO i VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // tworzenie drugiego VBO i VAO
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // odkomentuj tê linijkê, aby rysowaæ w trybie wyœwietlania linii.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // pêtla renderowania
    // -----------------
    while (!glfwWindowShouldClose(window))
    {
        // obs³uga wejœcia
        // ---------------
        processInput(window);

        // renderowanie
        // -----------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // rysowanie drugiego trójk¹ta
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: zamiana buforów i obs³uga zdarzeñ wejœciowych (naciœniêcie/wyciœniêcie klawiszy, ruch myszy itp.)
        // ---------------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // opcjonalne: zwolnienie wszystkich zasobów, gdy nie s¹ ju¿ potrzebne:
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    // glfw: zakoñczenie, czyszczenie wszystkich wczeœniej zaalokowanych zasobów GLFW.
    // --------------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// obs³uga wejœcia: sprawdzenie, czy odpowiednie klawisze s¹ naciœniête/wyciœniête w tej klatce i reakcja na to
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: funkcja wywo³ywana za ka¿dym razem, gdy zmieni siê rozmiar okna (zmiana rozmiaru przez OS lub u¿ytkownika)
// --------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // upewnij siê, ¿e viewport pasuje do nowych wymiarów okna; zauwa¿, ¿e szerokoœæ i wysokoœæ bêd¹ znacznie wiêksze ni¿ podane na wyœwietlaczach retina.
    glViewport(0, 0, width, height);
}

/*

framebuffer_size_callback(GLFWwindow* window, int width, int height): 
Jest to funkcja zwrotna (callback), która jest wywo³ywana przez bibliotekê GLFW za ka¿dym razem, gdy zmienia siê rozmiar okna. 
Funkcja ta ustawia widok OpenGL (viewport) na nowe wymiary okna.

processInput(GLFWwindow* window): Ta funkcja sprawdza, czy u¿ytkownik nacisn¹³ klawisz Esc. 
Jeœli tak, ustawia flagê zamykania okna GLFW, co powoduje zakoñczenie g³ównej pêtli programu.

main(): G³ówna funkcja programu. Odpowiada za inicjalizacjê GLFW, tworzenie okna, inicjalizacjê i ³adowanie funkcji OpenGL przy u¿yciu GLAD, 
kompilacjê i linkowanie shaderów, konfiguracjê danych wierzcho³ków, oraz uruchamianie g³ównej pêtli renderowania.

Inicjalizacja GLFW i konfiguracja kontekstu OpenGL.
Tworzenie okna GLFW.
Inicjalizacja GLAD w celu ³adowania funkcji OpenGL.
Kompilacja shaderów wierzcho³ków i fragmentów oraz ich linkowanie w program shaderów.
Konfiguracja danych wierzcho³ków poprzez tworzenie i konfigurowanie buforów wierzcho³ków i atrybutów wierzcho³ków.
G³ówna pêtla renderowania, w której odbywa siê obs³uga wejœcia, czyszczenie bufora kolorów, u¿ycie programu shaderów, rysowanie trójk¹tów i obs³uga zdarzeñ wejœciowych.
Zwolnienie zasobów po zakoñczeniu programu.

*/

