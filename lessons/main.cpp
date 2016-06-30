/* 
	Данный код является результатом уроков по OpenGL, и он помогает понять базовые принципы работы с OpenGL.
	Так как код является по большей части наглядным пособием, он изобилует комментариями разной степенью очевидности, вплоть до "это и ежу ясно". Порою.
	А порою комментарии отстутствуют там, где они очень нужны и "без поллитры не разберешься". 
	Я предупредил.
	Оригинальный сайт с уроками: http://ogldev.atspace.co.uk/index.html
	Переводы: http://triplepointfive.github.io/ogltutor/
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h> /* 
						Обеспечивает эффективные run-time механизмы для определения того, какие OpenGL расширения поддерживаются на целевой платформе. 
						Если вы подключаете другие заголовки OpenGL, вам следует подключить его раньше остальных, иначе GLEW откажется работать. 
					*/
#include <GL/freeglut.h> /* 
						API для управления оконной системой, а так же обработка событий, контроль ввода/вывода и ещё несколько других возможностей 
					*/

#include "pipeline.h" /* 
						Кастомный заголовок, постепенно увеличивающийся в ходе обучения.
						Он содержит фиксированный конвеер функций, необходимых для работы с графическими объектами.
					*/
#include "camera.h" /*
						Это объявление заголовочника класса камеры. 
						Он хранит 3 свойства, которые характеризуют камеру - позиция, направление и верхний вектор. 
						Так же добавлены 2 конструктора. 
						По умолчанию просто располагает камеру в начале координат, 
						направляет ее в сторону уменьшения Z, а верхний вектор устремляет в “небо” (0,1,0). 
						Но есть возможность создать камеру с указанием значений атрибутов.
					*/
#include "texture.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};

GLuint VBO; // Глобальная переменная для хранения указателя на буфер вершин
GLuint IBO; // То же самое для буфера индексов этих вершин
GLuint gWVPLocation; /*
							Мы используем этот указатель для доступа к всемирной матрице, 
							представленной в виде uniform-переменной внутри шейдера. 
							Всемирная она потому, что всё что мы делаем с объектом, это изменение его позиции в место, 
							которое мы указываем относительно координатной системы внутри нашего виртуального ‘мира’.
					   */
GLuint gSampler;
Texture* pTexture = NULL;
Camera* pGameCamera = NULL;

/*
	Из-за технических трудностей нижеследующий код шейдеров закомментировать не удалось.
	Тем не менее, общая информация по синтаксису успешно гуглится (нет), так что не все так плохо.
	Сорян =(
*/
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout (location = 1) in vec2 TexCoord;                                             \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec2 TexCoord0;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
    TexCoord0 = TexCoord;                                                           \n\
}"; /*Вершинный шейдер*/

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec2 TexCoord0;                                                                  \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
uniform sampler2D gSampler;                                                         \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = texture2D(gSampler, TexCoord0.xy);                                  \n\
}"; /*Пиксельный шейдер*/


static void RenderSceneCB()
{
	pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	Scale += 0.1f;

    Pipeline p;
	p.Rotate(0.0f, 0.0f, 0.0f);
    p.WorldPos(0.0f, 0.0f, 3.0f);
	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
	p.SetPerspectiveProj(60.0, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans()); // Плавно и красиво изменяем отображение фигуры на экране

	glEnableVertexAttribArray(0); // Разрешаем использование атрибута вершины для доступа к нему через конвеер. Это необходимо для шейдеров
	glEnableVertexAttribArray(1); // Разрешаем использование атрибутов вершин 1 для координат текстур в дополнении к атрибуту 0, который уже занят для позиции.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем указатель на вершины для отрисовки кадра
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Этот вызов говорит конвейеру как воспринимать данные внутри буфера вешин.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // Тоже самое для буфера текстур
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Привязываем указатель на индексы для отрисовки кадра
	pTexture->Bind(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0); /*
										Это признак хорошего тона отключать каждый атрибут вершины, как только отпадает необходимость в нем. 
										Оставить его включенным, пока шейдер не используется - лучший способ получить проблем.
								   */
	glDisableVertexAttribArray(1);

	glutSwapBuffers(); /*
							После рисования нужно просить OpenGL сменить экранные буфера при помощи glutSwapBuffers (), ведь у нас включена двойная буферизация. 
							Все рисуется на скрытом от пользователя буфере и затем происходит смена буферов. 
							Делается это для получения плавной анимации и для того, чтобы не было эффекта мерцания экрана.
					   */
}


static void SpecialKeyboardCB(int Key, int x, int y)
{
	pGameCamera->OnKeyboard(Key);
} // Функция отправляет клавишу и позицию курсора в момент нажатия кнопки.

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 27 : // 27 == ESC
		glutLeaveMainLoop();
	}
}

static void PassiveMouseCB(int x, int y)
{
	pGameCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB); // Указываем функцию рендера в качестве "ленивой"
	glutSpecialFunc(SpecialKeyboardCB); /*
											Эта функция записывает нашу для вызова, если будет нажата “специальная” клавиша. 
											В группу специальных клавиш входят: F{1..12}, стрелки и PAGE-UP / PAGE-DOWN / HOME / END / INSERT. 
											Если вам нужны обычные кнопки (символы и цифры) используйте glutKeyboardFunc().
										*/
	glutPassiveMotionFunc(PassiveMouseCB); // Пассивное движение означает, что мышь движется без нажатия каких-либо кнопок.
	glutKeyboardFunc(KeyboardCB);
}

static void CreateVertexBuffer()
{
	Vertex Vertices[8];
	Vertices[0] = Vertex(Vector3f(0.5, 0.5, -0.5), Vector2f(0.0, 0.0));
	Vertices[1] = Vertex(Vector3f(0.5, -0.5, -0.5), Vector2f(1.0, 0.0));
	Vertices[2] = Vertex(Vector3f(-0.5, -0.5, -0.5), Vector2f(1.0, 1.0));
	Vertices[3] = Vertex(Vector3f(-0.5, 0.5, -0.5), Vector2f(0.0, 1.0));
	Vertices[4] = Vertex(Vector3f(0.5, 0.5, 0.5), Vector2f(1.0, 0.0));
	Vertices[5] = Vertex(Vector3f(0.5, -0.5, 0.5), Vector2f(1.0, 1.0));
	Vertices[6] = Vertex(Vector3f(-0.5, -0.5, 0.5), Vector2f(0.0, 1.0));
	Vertices[7] = Vertex(Vector3f(-0.5, 0.5, 0.5), Vector2f(0.0, 0.0));

	glGenBuffers(1, &VBO); // Создаем буфер в общем типе. Для указания задачи используется следующая функция.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем указатель для наполнения данными
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // После связывания нашего объекта, мы наполняем его данными.
}

static void CreateIndexBuffer()
{
	unsigned int Indices[] = {	0, 1, 2,
								2, 3, 0,
								0, 4, 3,
								3, 7, 4,
								4, 0, 1,
								1, 4, 5,
								5, 1, 2,
								2, 5, 6,
								6, 2, 3,
								3, 6, 7,
								7, 4, 5,
								5, 6, 7 };

	glGenBuffers(1, &IBO); // Здесь и ниже: всё аналогично CreateVertexBuffer()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(1);
	}

	const GLchar* shdrTxt[1];
	shdrTxt[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, shdrTxt, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj); // Присоединяем скомпилированный объект шейдера к объекту программы
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram(); /*
													Мы начинаем процесс разработки шейдеров через создание программного объекта. 
													Позже мы слинкуем все шейдеры в этот объект.
											  */

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER); // Создание вершинного шейдера
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER); // Создание фрагментного (пиксельного) шейдера

	GLint success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram); /*
										Для использования отлинкованной программы шейдеров мы назначаем её для конвейера используя вызов выше. 
										Эта программа сохранит эффект для всех вызовов отрисовки, пока вы не замените её другой 
										или не запретите её использование напрямую функцией glUseProgram с параметром NULL. 
										Если вы создадите шейдерную программу, содержащую только 1 тип шейдеров, 
										тогда другие этапы будут использовать свою функциональность по-умолчанию.
								 */

	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP"); // Запрашиваем позицию uniform-переменной gWP в программном объекте
	assert(gWVPLocation != 0xFFFFFFFF); /*
												Очень важна проверка на ошибки (как мы и сделали тут), иначе обновления переменной не попадут в шейдер. 
												Есть 2 основные причины ошибки у этой функции. 
												Вы написали с ошибкой имя переменной или она была убрана компилятором с целью оптимизации. 
												Если компилятор не обнаружит использования переменной, он без раздумий выбросит её. 
												В этом случае glGetUniformLocation не даст результата.
										  */
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler"); // Аналогично с gSampler
	assert(gSampler != 0xFFFFFFFF);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Инициализируем GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*
														Так настраиваются некоторые опции GLUT. 
														GLUT_DOUBLE включает двойную буферизацию (отрисовка будет происходить в фоновый буфер, в то время как другой буфер отображается) 
														и буфер цвета, непрерывно используемого для конечной цели рендеринга (т.е. на экране).
												  */
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial XX"); // Здесь задается заголовок окна. В оригинальных уроках у каждого туториала есть индивидуальный двузначный номер. Здесь он заменен на ХХ.
	glutGameModeString("800x600@32"); // Эта функция glut’а разрешает вашему приложению запускаться в полноэкранном режиме, называемом как ‘игровой режим’. 
	//glutEnterGameMode();

	InitializeGlutCallbacks(); // Большая часть взаимодействий с системой происходит через функции обратного вызова

	pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	/* Теперь мы инициализируем GLEW и проверяем на ошибки. GLUT необходимо инициализировать раньше. */
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Установка "чистого" цвета фона
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders(); // Компилируем шейдеры

	glUniform1i(gSampler, 0);

	pTexture = new Texture(GL_TEXTURE_2D, "../thirdparty/content/test.png");
	//pTexture = new Texture(GL_TEXTURE_2D, "thirdparty/content/test.png");

	if (!pTexture->Load()) {
		return 1;
	}

	glutMainLoop(); /*
						Этот вызов передаёт контроль GLUT’у, который теперь начнёт свой собственный цикл. 
						В этом цикле он ждёт событий от оконной системы и передаёт их через функции обратного вызова, которые мы задали ранее.
					*/

	system("Pause");
	return 0;
}
