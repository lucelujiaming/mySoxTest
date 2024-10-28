package com.jiaming.mycompsciproblems;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by lucelu on 2024/10/23.
 */

public class MyTilesView extends View {

    private final int TILE_START_XPOS = 50;
    private final int TILE_START_YPOS = 50;
    private final int TILE_WIDTH = 30;
    private final int TILE_HEIGHT = 30;

    // 最大和最小宽高
    private final int VIEW_MIN_WIDTH  = 100; // 最小宽度
    private final int VIEW_MIN_HEIGHT  = 100; // 最小高度
    private final int VIEW_MAX_WIDTH  = TILE_START_XPOS * 2 + 25 * TILE_WIDTH;  // 最大宽度
    private final int VIEW_MAX_HEIGHT  = TILE_START_YPOS * 2 + 25 * TILE_HEIGHT; // 最大高度

    private Paint paintEmpty;
    private Paint paintBlocked;
    private Paint paintStart;
    private Paint paintGoal;
    private Paint paintPath;

    private Paint paintEdge;

    private Maze.Cell[][] grid;

    private int rows, columns;

    public MyTilesView(Context context) {
        super(context);
        init();
    }

    public MyTilesView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    public MyTilesView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        paintEmpty = new Paint();
        paintEmpty.setColor(Color.WHITE); // 设置矩形的颜色
        paintEmpty.setStyle(Paint.Style.FILL); // 设置填充样式

        paintBlocked = new Paint();
        paintBlocked.setColor(Color.BLACK); // 设置矩形的颜色
        paintBlocked.setStyle(Paint.Style.FILL); // 设置填充样式

        paintStart = new Paint();
        paintStart.setColor(Color.RED); // 设置矩形的颜色
        paintStart.setStyle(Paint.Style.FILL); // 设置填充样式

        paintGoal = new Paint();
        paintGoal.setColor(Color.GREEN); // 设置矩形的颜色
        paintGoal.setStyle(Paint.Style.FILL); // 设置填充样式

        paintPath = new Paint();
        paintPath.setColor(Color.BLUE); // 设置矩形的颜色
        paintPath.setStyle(Paint.Style.FILL); // 设置填充样式

        paintEdge = new Paint();
        paintEdge.setColor(Color.MAGENTA); // 设置矩形的颜色
        paintEdge.setStyle(Paint.Style.FILL); // 设置填充样式
        paintEdge.setStrokeWidth(5);

        grid = null;
        rows = 0;
        columns = 0;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawLine(TILE_START_XPOS, TILE_START_YPOS,
                TILE_START_XPOS + columns * TILE_WIDTH , TILE_START_YPOS, paintEdge);
        canvas.drawLine(TILE_START_XPOS, TILE_START_YPOS,
                TILE_START_XPOS , TILE_START_YPOS + rows * TILE_HEIGHT, paintEdge);
        canvas.drawLine(TILE_START_XPOS + columns * TILE_WIDTH , TILE_START_YPOS,
                TILE_START_XPOS + columns * TILE_WIDTH , TILE_START_YPOS + rows * TILE_HEIGHT, paintEdge);
        canvas.drawLine(TILE_START_XPOS , TILE_START_YPOS + rows * TILE_HEIGHT,
                TILE_START_XPOS + columns * TILE_WIDTH , TILE_START_YPOS + rows * TILE_HEIGHT, paintEdge);

        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                float left = column * TILE_WIDTH + TILE_START_XPOS;
                float top = row * TILE_HEIGHT + TILE_START_YPOS;
                float right = (column + 1) * TILE_WIDTH + TILE_START_XPOS;
                float bottom = (row  + 1) * TILE_HEIGHT + TILE_START_YPOS;
                // 绘制矩形
                if(grid[row][column] == Maze.Cell.BLOCKED) {
                    canvas.drawRect(left, top, right, bottom, paintBlocked);
                }
                else if(grid[row][column] == Maze.Cell.START) {
                    canvas.drawRect(left, top, right, bottom, paintStart);
                }
                else if(grid[row][column] == Maze.Cell.GOAL) {
                    canvas.drawRect(left, top, right, bottom, paintGoal);
                }
                else if(grid[row][column] == Maze.Cell.PATH) {
                    canvas.drawRect(left, top, right, bottom, paintPath);
                }
                else if(grid[row][column] == Maze.Cell.EMPTY) {
                    canvas.drawRect(left, top, right, bottom, paintEmpty);
                }
            }
        }
        // 设置最终的宽和高
        setMeasuredDimension(TILE_START_XPOS + columns * TILE_WIDTH , TILE_START_YPOS + rows * TILE_HEIGHT);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        // super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        // 获取建议的宽度和高度
        int width = MeasureSpec.getSize(widthMeasureSpec);
        int height = MeasureSpec.getSize(heightMeasureSpec);

        // 应用最大和最小值
        width = Math.max(VIEW_MIN_WIDTH, Math.min(width, VIEW_MAX_WIDTH));
        height = Math.max(VIEW_MIN_HEIGHT, Math.min(height, VIEW_MAX_HEIGHT));

        // 设置最终的宽和高
        setMeasuredDimension(width, height);
    }

    public boolean setTilesGrid(Maze.Cell[][] grid, int rows, int columns) {
        this.grid = null;
        this.rows = rows;
        this.columns = columns;
        // System.gc();

        this.grid = new Maze.Cell[rows][columns];
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                this.grid[row][column] = grid[row][column];
            }
        }
        return true;
    }
}
