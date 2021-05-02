/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sh1106.h"
#include "font5x7.h"
#include "font7x10.h"
#include "bitmap.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
volatile int speed_cactus = 0;
volatile int speed_dino = 0;
volatile int speed_score_and_display = 0;
volatile int speed_sword = 0;
volatile int speed_bird = 0;
volatile int speed_bird_wing = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
struct dino
{
  int x_dino, y_dino;
  int dino_status;
  int level_jump_dino;
  int fl_dino_foot;
};

struct cactus
{
  int x_cactus, y_cactus;
  int fl_cactus_destroy;
};

struct bird
{
  int x_bird, y_bird;
  int bird_wing;
};

struct sword
{
  int x_sword, y_sword;
  int fl_dino_have_sword;
  int durable_sword;
  int fl_sword_must_appear;
  int fl_sword;
};

struct dino dino1;
struct cactus cactus1;
struct sword sword1;
struct bird bird1;
int32_t score = 0, hscore = 0; //start point, highest point
int fl_invert = 1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  speed_cactus++;
  speed_dino++;
  speed_score_and_display++;
  speed_sword++;
  speed_bird++;
  speed_bird_wing++;
}

void dino_work()
{
  //dino position
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_RESET || dino1.y_dino != 38)
  //when play push button or dino is jumping/downing
  {

    if ((dino1.y_dino == 28) && (dino1.level_jump_dino == -2)) // increase/decrese jump speed like inertia
      dino1.level_jump_dino = -1;
    if ((dino1.y_dino == 24) && (dino1.level_jump_dino == 1))
      dino1.level_jump_dino = 2;

    if (dino1.y_dino == 38) // 38 is default position (when dino in ground)
    {
      dino1.level_jump_dino = -2; // dino jumping
    }
    if (dino1.y_dino == 12) //12 is the highest position when dino jumping
    {
      dino1.level_jump_dino = 1; // dino downing
    }
    dino1.y_dino = dino1.y_dino + dino1.level_jump_dino; //y_dino change
  }

  //    //foot dino
  //    if (dino1.fl_dino_foot == 1)
  //    {
  //    	dino1.fl_dino_foot=0;
  //    }
  //    else if (dino1.fl_dino_foot == 0)
  //    	dino1.fl_dino_foot=1;

  //dino dead
  if ((cactus1.x_cactus > 5) && (cactus1.x_cactus < 24) && (dino1.y_dino > 34) && (cactus1.fl_cactus_destroy == 0))
  {
    //if cactus.x_cactus in 5-> 24 and dino don't jump and cactus not be destroyed
    dino1.dino_status = 1; //dino dead
  }

  if ((bird1.x_bird > 5) && (bird1.x_bird < 24) && (dino1.y_dino < 20))
  {
    //if bird in 5-> 24 and dino is jumping and cactus not be destroyed
    dino1.dino_status = 1; //dino dead
  }
}

void sword_work()
{
  /////////////////////////////Sword appear////////////////////////////////////////////////////
  if ((score != 0) && (score % 800 == 0) && (sword1.fl_dino_have_sword == 0))
  {
    sword1.fl_sword_must_appear = 1; //if dino dont have sword and score % 80 = 0 -> sword appear
    sword1.x_sword = 115;
  }

  if ((speed_sword > 200) && (sword1.fl_sword_must_appear == 1))
  {
    speed_sword = 0;
    sword1.x_sword--;//sword position
    if (sword1.x_sword <= 1)           //if sword display at end of screen but dino dont eat
      sword1.fl_sword_must_appear = 0; //-> sword disappear
  }

  //position dino: x= 10->30
  if ((sword1.fl_sword_must_appear == 1) && (10 < sword1.x_sword) && (sword1.x_sword < 30) && (18 < dino1.y_dino) && (dino1.y_dino < 22))
  {
    sword1.fl_sword_must_appear = 0;
    sword1.fl_dino_have_sword = 1;
    sword1.durable_sword = 1000;
  }

  ///////////////////////////////Sword cut///////////////////////////////////////////////
  if ((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_RESET) && (sword1.fl_dino_have_sword == 1))
  {
    sword1.durable_sword = sword1.durable_sword - 5;
    {
      sword1.fl_sword = 0; //cut down
    }
  }
  else
    sword1.fl_sword = 1; //normal

  //sword cut effect position: x= 25 -> 44

  if ((sword1.fl_sword == 0) && (25 < cactus1.x_cactus) && (cactus1.x_cactus < 44) && (dino1.y_dino == 38) && (sword1.fl_dino_have_sword == 1) && (sword1.durable_sword > 0))
    // if dino cut a cactus when dino stand on ground (dino not jumping)
    cactus1.fl_cactus_destroy = 1; //cactus be destroyed

  if (sword1.durable_sword <= 0)
  {
    sword1.fl_dino_have_sword = 0;
    sword1.durable_sword = 0;
  }
}

void cactus_work()
{
  //////////////////////////////////Cactus////////////////////////////////////////////////////////////////////////
  int speed_cactus_now;
  //////////////////////Increase speed cactus by score
  if (score <= 1000) // score display= score /10
    speed_cactus_now = 140;
  else if (score <= 2000)
    speed_cactus_now = 100;
  else if (score <= 3000)
    speed_cactus_now = 80;
  else if (score > 3000)
    speed_cactus_now = 40;


  if (speed_cactus > speed_cactus_now)
  {
    speed_cactus = 0;
    cactus1.x_cactus--;
    if (cactus1.x_cactus == 0) //after cactus go to the end of display  (x display max=128)
    {
      cactus1.x_cactus = 122; //cactus appear at start of display
      cactus1.fl_cactus_destroy = 0;
    }
  }
}

void bird_work()
{
  //////////////////////////////////Bird////////////////////////////////////////////////////////////////////////
  int speed_bird_now;
  //////////////////////Increase speed bird by score
  if (score < 4000) // score display= score /10
    speed_bird_now = 200;
  else if (score <= 5000)
    speed_bird_now = 150;
  else if (score <= 6000)
    speed_bird_now = 80;
  else if (score > 6000)
    speed_bird_now = 40;


  if (speed_bird > speed_bird_now)
  {
    speed_bird = 0;
    bird1.x_bird--;
    if (bird1.x_bird == 0) //after bird go to the end of display  (x display max=128)
    {
      bird1.x_bird = 122; //bird appear at start of display
    }
  }
}

void bird_wing_work()
{
  if (speed_bird_wing > 150)
  {
    speed_bird_wing = 0;

    if (bird1.bird_wing == 0)
    {
      bird1.bird_wing = 1;
    }
    else if (bird1.bird_wing == 1)
    {
      bird1.bird_wing = 0;
    }
  }
}

void display()
{

  /////////////////Display//////////////////////////////////////////////////////////
  SH1106_Fill(0x00);                     //clear screen
  LCD_DrawBitmap(1, 58, 127, 8, ground); //display ground
  LCD_HLine(1, 127, 58);                 ////display line ground

  if (cactus1.fl_cactus_destroy == 0)
    LCD_DrawBitmap(cactus1.x_cactus, cactus1.y_cactus, 6, 16, cactus); //display cactus
  else
    LCD_DrawBitmap(cactus1.x_cactus, cactus1.y_cactus + 9, 3, 7, cactus_dead3x7);

  if ((bird1.bird_wing == 1) && (score > 2300))
    LCD_DrawBitmap(bird1.x_bird, bird1.y_bird, 14, 12, bird_down_14x12);
  else if ((bird1.bird_wing == 0) && (score > 2300))
    LCD_DrawBitmap(bird1.x_bird, bird1.y_bird - 6, 14, 12, bird_up_14x12); //display bird

  if (dino1.dino_status == 1) //dino dead
    LCD_DrawBitmap(dino1.x_dino, dino1.y_dino, 20, 20, googledino20x20_dead);
  else if (dino1.fl_dino_foot == 1)
    LCD_DrawBitmap(dino1.x_dino, dino1.y_dino, 20, 20, googledino20x20_right);
  else if (dino1.fl_dino_foot == 0)
    LCD_DrawBitmap(dino1.x_dino, dino1.y_dino, 20, 20, googledino20x20_left); //display dino

  if ((sword1.fl_sword == 1) && (sword1.fl_dino_have_sword == 1))
    LCD_DrawBitmap(dino1.x_dino + 15, dino1.y_dino - 1, 13, 13, sword13x13_up); //display sword
  else if ((sword1.fl_sword == 0) && (sword1.fl_dino_have_sword == 1))
    LCD_DrawBitmap(dino1.x_dino + 15, dino1.y_dino + 6, 19, 7, sword19x7_down);

  if (sword1.fl_sword_must_appear == 1)
    LCD_DrawBitmap(sword1.x_sword, sword1.y_sword, 13, 13, sword13x13_up); //display sword to eat

  LCD_PutStr(85, 2, " HI:", &Font5x7);
  LCD_PutInt(5, 2, score / 10, &Font5x7);    //display score
  LCD_PutInt(110, 2, hscore / 10, &Font5x7); //display highest score

  if (sword1.durable_sword > 0) //display dino have sword time
  {
    LCD_PutInt(50, 2, sword1.durable_sword / 10, &Font5x7);
    LCD_PutStr(65, 2, "%", &Font5x7);
  }

  if ((score % 1000 == 0) && (fl_invert == 1)) //change day and night
  {
    SH1106_SetInvert(0);
    fl_invert = 0;
  }
  else if ((score % 1000 == 0) && (fl_invert == 0))
  {
    SH1106_SetInvert(1);
    fl_invert = 1;
  }
  SH1106_Flush();
}

void dino_dead()
{
  LCD_PutStr(30, 20, "GAME OVER", &Font7x10);
  SH1106_Flush();

  while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_SET)
    ; //if push button again
      //reset game
  cactus1.x_cactus = 122;
  cactus1.y_cactus = 42; //start position cactus
  cactus1.fl_cactus_destroy = 0;

  bird1.x_bird = 110;
  bird1.y_bird = 15;
  bird1.bird_wing = 0; //wing up

  sword1.x_sword = 0;
  sword1.y_sword = 20; //sword appear position
  sword1.fl_sword = 1;
  sword1.fl_dino_have_sword = 0; //dino do not have sword
  sword1.durable_sword = 0;
  sword1.fl_sword_must_appear = 0;

  dino1.x_dino = 10;
  dino1.y_dino = 38;          //start position dino 20*20
  dino1.level_jump_dino = -2; //level dino jump/drop
  dino1.dino_status = 0;      // Dino dead or alive, 0= alive, 1=dead
  dino1.fl_dino_foot = 1;

  SH1106_SetInvert(1);
  fl_invert = 1;
  if (score > hscore)
  {
    hscore = score;
  }
  score = 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  SH1106_Init();
  HAL_TIM_Base_Init(&htim1);
  HAL_TIM_Base_Start_IT(&htim1);

  SH1106_SetInvert(1);
  SH1106_Contrast(100);
  SH1106_Orientation(0);
  SH1106_Flush();

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

  cactus1.x_cactus = 122;
  cactus1.y_cactus = 42; //start position cactus
  cactus1.fl_cactus_destroy = 0;

  bird1.x_bird = 110;
  bird1.y_bird = 15;
  bird1.bird_wing = 0; //wing up

  sword1.x_sword = 0;
  sword1.y_sword = 20; //sword appear position
  sword1.fl_sword = 1;
  sword1.fl_dino_have_sword = 0; //dino do not have sword
  sword1.durable_sword = 0;
  sword1.fl_sword_must_appear = 0;

  dino1.x_dino = 10;
  dino1.y_dino = 38;          //start position dino 20*20
  dino1.level_jump_dino = -2; //level dino jump/drop
  dino1.dino_status = 0;      // Dino dead or alive, 0= alive, 1=dead
  dino1.fl_dino_foot = 1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (speed_dino > 80)
    {
      speed_dino = 0;
      dino_work();
    }

    sword_work();

    cactus_work();

    if (score > 2300)
    {
      bird_work();
      bird_wing_work();
    }

    if (speed_score_and_display > 50)
    {
      speed_score_and_display = 0;
      score++;
      display();
    }

    if (dino1.dino_status == 1)
    {
      dino_dead();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 200;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CS_Pin | DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON_2_Pin BUTTON_1_Pin */
  GPIO_InitStruct.Pin = BUTTON_2_Pin | BUTTON_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_Pin DC_Pin */
  GPIO_InitStruct.Pin = CS_Pin | DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : OLED_RESET_Pin */
  GPIO_InitStruct.Pin = OLED_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OLED_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
