using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class BackGround : MonoBehaviour
{
    static int nowStageNum;

    public static int GetStageNum()
    {
        return nowStageNum;
    }

    void Update()
    {
        if (SceneManager.GetActiveScene().name == "Stage1")
        {
            nowStageNum = 1;
        }
        else if (SceneManager.GetActiveScene().name == "Stage2")
        {
            nowStageNum = 2;
        }
        else if (SceneManager.GetActiveScene().name == "Stage3")
        {
            nowStageNum = 3;
        }
        else if (SceneManager.GetActiveScene().name == "Stage4")
        {
            nowStageNum = 4;
        }
        else if (SceneManager.GetActiveScene().name == "Stage5")
        {
            nowStageNum = 5;
        }
        else if (SceneManager.GetActiveScene().name == "Stage6")
        {
            nowStageNum = 6;
        }
        else if (SceneManager.GetActiveScene().name == "Stage7")
        {
            nowStageNum = 7;
        }
        else if (SceneManager.GetActiveScene().name == "Stage8")
        {
            nowStageNum = 8;
        }
    }
}