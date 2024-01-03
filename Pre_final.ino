#define COLOUR_DEBTH 2
#define CRT_SQUARE
#include <microLED.h>
#define count_LED 59 // сюда кол во чипов не ленте
#define pin_LED 2  // сюда пин в который подключена лента

unsigned long time;

unsigned long low_random_border = 0; // Нижняя граница генерации зоны пульсации
unsigned long top_random_border = count_LED; // Верхняя граница генерации зоны пульсации
int top_border_width; // Нижняя граница пульсирующей зоны
int low_border_width; // Верхняя граница пульсирующей зоны
int top_brightness = 255; // Максимальная яркость
int saturation = 225; // Насыщенность


microLED<count_LED, pin_LED, -1, LED_WS2812, ORDER_GRB> strip; // order_rgb - порядок располодения цветов на ленте.

void setup() {

    pinMode(2, 1);  // Управляющий пин на ленту

//----Заливка цветом--------------------------------------------------
    mData color =  mHSV(25, saturation, top_brightness); // Установка стартового цвета (цвет/насыщенность/яркость).
    strip.fill (color); // Заливка ленты стартовым цветом
    
//----Установка начальной яркости--------------------------------------
    strip.setBrightness(top_brightness); // Установка начальной яркости
    strip.show(); // Вывод всех эффектов на ленту

}

void loop() {

    random_width(); // Генерация ширины участков пульсации
    pulse(); // Пульсация сгенерированного учатска по цвету и свету

}

void random_width() {
    /*  
        Функция генерирует рандомную точку на ленте.
        Справа и слева от точки добавляет от 0 до 5,
        Таким образом получается ширина пульсирубщей зоны.
    */
    int middle_random_point = random(low_random_border, top_random_border); // Генерация точки, вокруг которой строится пульс-зона 
    unsigned long top_diff_to_middle = 5; // Максимальное приращение вправо и влево.   
    unsigned long low_diff_to_middle = 0; // Минимальное приращение вправо и влево.

    int random_diff_to_middle = random(low_diff_to_middle, top_diff_to_middle); // Генерация приращения [0;5] 

    low_border_width = middle_random_point - random_diff_to_middle; // Нижняя граница зоны пульсации
    if (low_border_width < 0) { // Ограницение нижней границы, если она выходит за нижнюю границу ленты.
        low_border_width = 0;
    }

    top_border_width = middle_random_point + random_diff_to_middle; // Верхняя граница зоны пульсации
    if (top_border_width > top_random_border) { // Ограницение нижней границы, если она выходит за нижнюю границу ленты.
        top_border_width = top_random_border;
    }
} 

void pulse() {
    /*
        Функция генерирует нижнюю величину пульсации света [0;254]
        Генерирует ниюнюю величину пульсации цвета [20;26] 

        Пульсацию света с шагом diff_b = 1, прогоняет от top_brightness до low_brightness и обратно
        Пульсацию цвета гоняет по кругу от top_colour до low_colour, во время прохода пульсации света.

        Для каждым значением brighntess и colour проходит по зоне пульсации и установив их для каждого светодиода
            выводит полученный hsv цвет на нужном участке. 

        Повторяет, пока brightness не вернется к значению top_brightness 
    */
    //----рандомная генерация яркости-----------------------------
    unsigned long top_border_brightness = 255; // Верхняя граница генерации нижнего значения света
    unsigned long low_border_brightness = 50; // Нижняя граница генерации нижнего значения света

    int top_brightness = 255; // Верхняя граница пульсации света
    int low_brightness = random(low_border_brightness, top_border_brightness-1); // Нижняя граница пульсации света
    
    int brightness = top_brightness; // Начальное значение устанавливаемой яркости

    //----рандоманя генерация цвета--------------------------------
    unsigned long top_border_colour = 27;  // Тут все аналогично со светом, только для цвета
    unsigned long low_border_colour = 20;   

    int top_colour = 27;
    int colour = top_colour;
    int low_colour = random(low_border_colour, top_border_colour-1);

    //------------------------------------------------------------
    int diff_b = -1; // Шаг изменения яркости (увеличивая, увеличивается скорость эффекта)
    float diff_c = -0.20; // Шаг изменения цвета
    
    bool flag_to_reset_colour = 0;
    
    while (true) { 

        colour += diff_c;
        brightness += diff_b;

        if (brightness == top_brightness) {             
            break;
        }
        if (brightness < low_brightness) {
            brightness = low_brightness;
            diff_b = 1;
        }
        if (colour > top_border_colour) { // Ограничение коолебаниц цвета сверху
            colour = top_border_colour;
            diff_c *= -1;
        }
        if (colour < low_border_colour) {  // Ограничение коолебаниц цвета снизу
            colour = low_border_colour;
            diff_c *= -1;
        } 
        

        for (int i = low_border_width; i <= top_border_width; i++) { // Заполнение всего участка пульсации нынешним значением hsv цвета
            strip.set(i, mHSV(colour, saturation, brightness)); 
        }
        strip.show();
    }
}