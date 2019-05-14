using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameController2 : MonoBehaviour {

    public GameObject clear;

    public AudioClip sound;
    private AudioSource audioSource;

    int count = 0;
    int maxCount = 75;

    void Start()
    {
        audioSource = gameObject.GetComponent<AudioSource>();
    }

    void Update()
    {
        if (clear.activeSelf)
        {
            count++;


        }

        if (count >= maxCount)
        {
            SceneManager.LoadScene("Result");
        }
    }
}
