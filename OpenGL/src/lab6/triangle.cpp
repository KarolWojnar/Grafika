#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ustawienia
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// kod �r�d�owy shadera wierzcho�k�w
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// kod �r�d�owy shadera fragment�w
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
        std::cout << "Nie uda�o si� utworzy� okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: wczytanie wska�nik�w do funkcji OpenGL
    // -------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Nie uda�o si� zainicjowa� GLAD" << std::endl;
        return -1;
    }


    // kompilacja i zlinkowanie programu shader�w
    // -----------------------------------------
    // shader wierzcho�k�w
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // sprawdzenie b��d�w kompilacji shadera
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "B��D::SHADER::WIERZCHO�EK::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // shader fragment�w
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // sprawdzenie b��d�w kompilacji shadera
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "B��D::SHADER::FRAGMENT::KOMPILACJA_NIEUDANA\n" << infoLog << std::endl;
    }
    // zlinkowanie shader�w
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // sprawdzenie b��d�w linkowania
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "B��D::SHADER::PROGRAM::LINKOWANIE_NIEUDANE\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // konfiguracja danych wierzcho�k�w
    // --------------------------------
    float vertices1[] = {
        -0.4f, -0.6f, 0.0f, // lewy dolny punkt
         0.4f, -0.6f, 0.0f, // prawy dolny punkt
         0.0f,  0.0f, 0.0f  // g�rny punkt
    };

    float vertices2[] = {
        -0.4f, 0.6f, 0.0f, // lewy g�rny punkt
         0.4f, 0.6f, 0.0f, // prawy g�rny punkt
         0.0f, 0.0f, 0.0f  // g�rny punkt
    };

    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    // powi�zanie Vertex Array Object (VAO) jako pierwszego, nast�pnie powi�zanie i skonfigurowanie bufora wierzcho�k�w (VBO) i atrybut�w wierzcho�k�w
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

    // odkomentuj t� linijk�, aby rysowa� w trybie wy�wietlania linii.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // p�tla renderowania
    // -----------------
    while (!glfwWindowShouldClose(window))
    {
        // obs�uga wej�cia
        // ---------------
        processInput(window);

        // renderowanie
        // -----------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // rysowanie drugiego tr�jk�ta
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: zamiana bufor�w i obs�uga zdarze� wej�ciowych (naci�ni�cie/wyci�ni�cie klawiszy, ruch myszy itp.)
        // ---------------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // opcjonalne: zwolnienie wszystkich zasob�w, gdy nie s� ju� potrzebne:
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgram);

    // glfw: zako�czenie, czyszczenie wszystkich wcze�niej zaalokowanych zasob�w GLFW.
    // --------------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// obs�uga wej�cia: sprawdzenie, czy odpowiednie klawisze s� naci�ni�te/wyci�ni�te w tej klatce i reakcja na to
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: funkcja wywo�ywana za ka�dym razem, gdy zmieni si� rozmiar okna (zmiana rozmiaru przez OS lub u�ytkownika)
// --------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // upewnij si�, �e viewport pasuje do nowych wymiar�w okna; zauwa�, �e szeroko�� i wysoko�� b�d� znacznie wi�ksze ni� podane na wy�wietlaczach retina.
    glViewport(0, 0, width, height);
}

/*

framebuffer_size_callback(GLFWwindow* window, int width, int height): 
Jest to funkcja zwrotna (callback), kt�ra jest wywo�ywana przez bibliotek� GLFW za ka�dym razem, gdy zmienia si� rozmiar okna. 
Funkcja ta ustawia widok OpenGL (viewport) na nowe wymiary okna.

processInput(GLFWwindow* window): Ta funkcja sprawdza, czy u�ytkownik nacisn�� klawisz Esc. 
Je�li tak, ustawia flag� zamykania okna GLFW, co powoduje zako�czenie g��wnej p�tli programu.

main(): G��wna funkcja programu. Odpowiada za inicjalizacj� GLFW, tworzenie okna, inicjalizacj� i �adowanie funkcji OpenGL przy u�yciu GLAD, 
kompilacj� i linkowanie shader�w, konfiguracj� danych wierzcho�k�w, oraz uruchamianie g��wnej p�tli renderowania.

Inicjalizacja GLFW i konfiguracja kontekstu OpenGL.
Tworzenie okna GLFW.
Inicjalizacja GLAD w celu �adowania funkcji OpenGL.
Kompilacja shader�w wierzcho�k�w i fragment�w oraz ich linkowanie w program shader�w.
Konfiguracja danych wierzcho�k�w poprzez tworzenie i konfigurowanie bufor�w wierzcho�k�w i atrybut�w wierzcho�k�w.
G��wna p�tla renderowania, w kt�rej odbywa si� obs�uga wej�cia, czyszczenie bufora kolor�w, u�ycie programu shader�w, rysowanie tr�jk�t�w i obs�uga zdarze� wej�ciowych.
Zwolnienie zasob�w po zako�czeniu programu.

*/

