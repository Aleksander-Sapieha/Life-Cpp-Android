
package com.sourceguy.life_cpp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.*

class MainActivity : AppCompatActivity() {

    external fun nativeCreateGame(name: String): String
    external fun nativeStartDay(): String
    external fun nativeChooseAction(choice: Int): String
    external fun nativeChooseJob(choice: Int): String
    external fun nativeIsFinished(): Boolean

    companion object {
        init {
            System.loadLibrary("life_cpp")
        }
    }

    private lateinit var output: TextView
    private lateinit var buttons: LinearLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(32, 32, 32, 32)
        }

        val title = TextView(this).apply {
            text = "Life Simulator"
            textSize = 28f
        }

        output = TextView(this).apply {
            textSize = 17f
            setPadding(0, 24, 0, 24)
        }

        buttons = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
        }

        layout.addView(title)
        layout.addView(output)
        layout.addView(buttons)

        setContentView(layout)

        showStartScreen()
    }

    private fun addButton(label: String, action: () -> Unit) {
        val button = Button(this).apply {
            text = label
            setOnClickListener { action() }
        }

        buttons.addView(button)
    }

    private fun showStartScreen() {
        buttons.removeAllViews()

        val nameInput = EditText(this).apply {
            hint = "Enter your name"
        }

        buttons.addView(nameInput)

        addButton("Start Game") {
            val name = nameInput.text.toString().trim()

            if (name.isEmpty()) {
                nameInput.error = "Enter your name"
                return@addButton
            }

            nativeCreateGame(name)

            output.text = nativeStartDay()

            showActions()
        }
    }

    private fun showActions() {
        buttons.removeAllViews()

        val actions = listOf(
            "Work" to 1,
            "Relax" to 2,
            "Exercise" to 3,
            "Socialize" to 4,
            "Look for a job" to 5,
            "Buy a house" to 6,
            "Have a child" to 7
        )

        for ((label, choice) in actions) {
            addButton(label) {
                val result = nativeChooseAction(choice)
                output.text = result

                if (choice == 5 && !nativeIsFinished()) {
                    showJobs()
                } else {
                    checkGameState()
                }
            }
        }
    }

    private fun showJobs() {
        buttons.removeAllViews()

        addButton("Engineer") {
            output.text = nativeChooseJob(1)
            checkGameState()
        }

        addButton("Artist") {
            output.text = nativeChooseJob(2)
            checkGameState()
        }

        addButton("Teacher") {
            output.text = nativeChooseJob(3)
            checkGameState()
        }

        addButton("Back") {
            showActions()
        }
    }

    private fun checkGameState() {
        if (nativeIsFinished()) {
            buttons.removeAllViews()
            addButton("Play Again") {
                showStartScreen()
            }
        } else {
            showActions()
        }
    }
}
