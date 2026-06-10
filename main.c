#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_SHAPES 100

// Shape Types
typedef enum {
    SHAPE_LINE,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

// Shape Data Structures
typedef struct {
    int x1, y1;
    int x2, y2;
} LineData;

typedef struct {
    int x, y;
    int width, height;
} RectData;

typedef struct {
    int cx, cy;
    int radius;
} CircleData;

typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleData;

typedef struct {
    ShapeType type;
    union {
        LineData line;
        RectData rect;
        CircleData circle;
        TriangleData tri;
    } data;
} Shape;

// Global State
Shape shapes[MAX_SHAPES];
int shape_count = 0;
char canvas[HEIGHT][WIDTH];

// Function Prototypes
void clear_canvas();
void render_shapes();
void display_canvas();
void add_shape(Shape s);
void delete_shape(int index);
void list_shapes();
int read_int(const char* prompt, int min_val, int max_val);

void draw_line(int x1, int y1, int x2, int y2);
void draw_rectangle(int x, int y, int w, int h);
void draw_circle(int cx, int cy, int radius);

int main() {
    // Enable Virtual Terminal Processing for ANSI colors on Windows Console
    // (Sends ANSI escapes natively to clear and color text)
    #ifdef _WIN32
        // We use system call for console mode if supported, or just print standard menus.
        // For standard portability, standard ANSI codes will work in CMD/PowerShell out of the box.
    #endif

    char input_line[10];

    while (1) {
        printf("\n======= 2D Graphics Editor Menu =======\n");
        printf("1. Display Picture\n");
        printf("2. Add a Line\n");
        printf("3. Add a Rectangle\n");
        printf("4. Add a Circle\n");
        printf("5. Add a Triangle\n");
        printf("6. Delete an Object\n");
        printf("7. Clear Canvas (Delete All)\n");
        printf("8. Exit\n");
        printf("=======================================\n");

        int choice = read_int("Enter your choice (1-8): ", 1, 8);

        if (choice == 8) {
            printf("\nExiting 2D Graphics Editor. Goodbye!\n");
            break;
        }

        switch (choice) {
            case 1: { // Display Canvas
                render_shapes();
                display_canvas();
                break;
            }
            case 2: { // Line
                printf("\n--- Add a Line ---\n");
                int x1 = read_int("Enter Start X (0-79): ", 0, WIDTH - 1);
                int y1 = read_int("Enter Start Y (0-24): ", 0, HEIGHT - 1);
                int x2 = read_int("Enter End X (0-79): ", 0, WIDTH - 1);
                int y2 = read_int("Enter End Y (0-24): ", 0, HEIGHT - 1);

                Shape s;
                s.type = SHAPE_LINE;
                s.data.line.x1 = x1;
                s.data.line.y1 = y1;
                s.data.line.x2 = x2;
                s.data.line.y2 = y2;
                add_shape(s);
                printf("\nLine added successfully.\n");
                break;
            }
            case 3: { // Rectangle
                printf("\n--- Add a Rectangle ---\n");
                int x = read_int("Enter Top-Left corner X (0-79): ", 0, WIDTH - 1);
                int y = read_int("Enter Top-Left corner Y (0-24): ", 0, HEIGHT - 1);
                int w = read_int("Enter Width (1-80): ", 1, WIDTH - x);
                int h = read_int("Enter Height (1-25): ", 1, HEIGHT - y);

                Shape s;
                s.type = SHAPE_RECTANGLE;
                s.data.rect.x = x;
                s.data.rect.y = y;
                s.data.rect.width = w;
                s.data.rect.height = h;
                add_shape(s);
                printf("\nRectangle added successfully.\n");
                break;
            }
            case 4: { // Circle
                printf("\n--- Add a Circle ---\n");
                int cx = read_int("Enter Center X (0-79): ", 0, WIDTH - 1);
                int cy = read_int("Enter Center Y (0-24): ", 0, HEIGHT - 1);
                int radius = read_int("Enter Radius (1-40): ", 1, 40);

                Shape s;
                s.type = SHAPE_CIRCLE;
                s.data.circle.cx = cx;
                s.data.circle.cy = cy;
                s.data.circle.radius = radius;
                add_shape(s);
                printf("\nCircle added successfully.\n");
                break;
            }
            case 5: { // Triangle
                printf("\n--- Add a Triangle ---\n");
                int x1 = read_int("Enter Vertex 1 X (0-79): ", 0, WIDTH - 1);
                int y1 = read_int("Enter Vertex 1 Y (0-24): ", 0, HEIGHT - 1);
                int x2 = read_int("Enter Vertex 2 X (0-79): ", 0, WIDTH - 1);
                int y2 = read_int("Enter Vertex 2 Y (0-24): ", 0, HEIGHT - 1);
                int x3 = read_int("Enter Vertex 3 X (0-79): ", 0, WIDTH - 1);
                int y3 = read_int("Enter Vertex 3 Y (0-24): ", 0, HEIGHT - 1);

                Shape s;
                s.type = SHAPE_TRIANGLE;
                s.data.tri.x1 = x1;
                s.data.tri.y1 = y1;
                s.data.tri.x2 = x2;
                s.data.tri.y2 = y2;
                s.data.tri.x3 = x3;
                s.data.tri.y3 = y3;
                add_shape(s);
                printf("\nTriangle added successfully.\n");
                break;
            }
            case 6: { // Delete
                if (shape_count == 0) {
                    printf("\nNo objects to delete.\n");
                    break;
                }
                list_shapes();
                int idx = read_int("Enter the number of the shape to delete: ", 1, shape_count);
                delete_shape(idx - 1);
                printf("\nShape deleted successfully.\n");
                break;
            }
            case 7: { // Clear
                shape_count = 0;
                printf("\nCanvas cleared (all shapes deleted).\n");
                break;
            }
        }
    }

    return 0;
}

// Canvas & Shape list functions
void clear_canvas() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            canvas[y][x] = '_';
        }
    }
}

void render_shapes() {
    clear_canvas();
    for (int i = 0; i < shape_count; i++) {
        switch (shapes[i].type) {
            case SHAPE_LINE:
                draw_line(shapes[i].data.line.x1, shapes[i].data.line.y1,
                          shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                draw_rectangle(shapes[i].data.rect.x, shapes[i].data.rect.y,
                               shapes[i].data.rect.width, shapes[i].data.rect.height);
                break;
            case SHAPE_CIRCLE:
                draw_circle(shapes[i].data.circle.cx, shapes[i].data.circle.cy,
                            shapes[i].data.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                draw_line(shapes[i].data.tri.x1, shapes[i].data.tri.y1,
                          shapes[i].data.tri.x2, shapes[i].data.tri.y2);
                draw_line(shapes[i].data.tri.x2, shapes[i].data.tri.y2,
                          shapes[i].data.tri.x3, shapes[i].data.tri.y3);
                draw_line(shapes[i].data.tri.x3, shapes[i].data.tri.y3,
                          shapes[i].data.tri.x1, shapes[i].data.tri.y1);
                break;
        }
    }
}

void display_canvas() {
    // Clear screen escape sequence for nicer rendering loop
    printf("\033[H\033[J");
    printf("\n--- Canvas View (%dx%d) ---\n", WIDTH, HEIGHT);
    
    // Print column guide headers
    printf("    ");
    for (int x = 0; x < WIDTH; x++) {
        if (x % 10 == 0) printf("%d", x / 10);
        else printf(" ");
    }
    printf("\n    ");
    for (int x = 0; x < WIDTH; x++) {
        printf("%d", x % 10);
    }
    printf("\n");

    // Top border
    printf("   +");
    for (int x = 0; x < WIDTH; x++) printf("-");
    printf("+\n");

    // Canvas contents
    for (int y = 0; y < HEIGHT; y++) {
        printf("%2d |", y);
        for (int x = 0; x < WIDTH; x++) {
            if (canvas[y][x] == '*') {
                // Colored bold green '*' for shapes
                printf("\033[1;32m*\033[0m");
            } else {
                printf("_");
            }
        }
        printf("|\n");
    }

    // Bottom border
    printf("   +");
    for (int x = 0; x < WIDTH; x++) printf("-");
    printf("+\n");
}

void add_shape(Shape s) {
    if (shape_count < MAX_SHAPES) {
        shapes[shape_count++] = s;
    } else {
        printf("\nError: Maximum shape limit reached (%d).\n", MAX_SHAPES);
    }
}

void delete_shape(int index) {
    if (index < 0 || index >= shape_count) return;
    for (int i = index; i < shape_count - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    shape_count--;
}

void list_shapes() {
    printf("\nActive Shapes:\n");
    for (int i = 0; i < shape_count; i++) {
        printf("%d. ", i + 1);
        switch (shapes[i].type) {
            case SHAPE_LINE:
                printf("Line from (%d, %d) to (%d, %d)\n",
                       shapes[i].data.line.x1, shapes[i].data.line.y1,
                       shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                printf("Rectangle at (%d, %d), width %d, height %d\n",
                       shapes[i].data.rect.x, shapes[i].data.rect.y,
                       shapes[i].data.rect.width, shapes[i].data.rect.height);
                break;
            case SHAPE_CIRCLE:
                printf("Circle centered at (%d, %d) with radius %d\n",
                       shapes[i].data.circle.cx, shapes[i].data.circle.cy,
                       shapes[i].data.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                printf("Triangle vertices: (%d, %d), (%d, %d), (%d, %d)\n",
                       shapes[i].data.tri.x1, shapes[i].data.tri.y1,
                       shapes[i].data.tri.x2, shapes[i].data.tri.y2,
                       shapes[i].data.tri.x3, shapes[i].data.tri.y3);
                break;
        }
    }
}

// Robust input handling
int read_int(const char* prompt, int min_val, int max_val) {
    char line[128];
    long val;
    char* endptr;

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nInput ended (EOF). Exiting...\n");
            exit(0);
        }
        
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // Parse input string
        val = strtol(line, &endptr, 10);

        // Check if string was empty or not completely parsed
        if (endptr == line || *endptr != '\0') {
            printf("Invalid integer input. Try again.\n");
            continue;
        }

        // Check range limits
        if (val < min_val || val > max_val) {
            printf("Value out of range [%d, %d]. Try again.\n", min_val, max_val);
            continue;
        }

        break;
    }
    return (int)val;
}

// Shape rendering helper algorithms

// Bresenham's Line Algorithm (Integer-only, handles all slopes/directions)
void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            canvas[y1][x1] = '*';
        }
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Rectangle Outline drawing
void draw_rectangle(int x, int y, int w, int h) {
    // Horizontal edges
    for (int i = 0; i < w; i++) {
        int px = x + i;
        int py1 = y;
        int py2 = y + h - 1;
        if (px >= 0 && px < WIDTH) {
            if (py1 >= 0 && py1 < HEIGHT) canvas[py1][px] = '*';
            if (py2 >= 0 && py2 < HEIGHT) canvas[py2][px] = '*';
        }
    }
    // Vertical edges
    for (int i = 0; i < h; i++) {
        int py = y + i;
        int px1 = x;
        int px2 = x + w - 1;
        if (py >= 0 && py < HEIGHT) {
            if (px1 >= 0 && px1 < WIDTH) canvas[py][px1] = '*';
            if (px2 >= 0 && px2 < WIDTH) canvas[py][px2] = '*';
        }
    }
}

// Midpoint Circle Algorithm Helper
void plot_circle_points(int cx, int cy, int x, int y) {
    int points[8][2] = {
        {cx + x, cy + y}, {cx - x, cy + y},
        {cx + x, cy - y}, {cx - x, cy - y},
        {cx + y, cy + x}, {cx - y, cy + x},
        {cx + y, cy - x}, {cx - y, cy - x}
    };
    for (int i = 0; i < 8; i++) {
        int px = points[i][0];
        int py = points[i][1];
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            canvas[py][px] = '*';
        }
    }
}

// Midpoint Circle Algorithm
void draw_circle(int cx, int cy, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    plot_circle_points(cx, cy, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        plot_circle_points(cx, cy, x, y);
    }
}
