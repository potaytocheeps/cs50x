// Wait for DOM content to load
document.addEventListener('DOMContentLoaded', () => 
{
    // Get each section that will be enabled/disabled by buttons
    const carousel = document.querySelector('#carousel');
    const trailer = document.querySelector('#trailer');
    const synopsis = document.querySelector('#synopsis');

    // Get all buttons for displaying each section
    const buttons = document.querySelectorAll('.btn-outline-light');

    // When a section button is clicked, display the section outlined by that button and hide
    // content of any other section that was previously displayed
    document.querySelector('#images-button').addEventListener('click', (event) =>
    {
        trailer.style.display = 'none';
        synopsis.style.display = 'none';
        carousel.style.display = 'flex';

        buttons.forEach((button) =>
        {
            addButtonActivity(button, event)
        });
    });

    document.querySelector('#trailer-button').addEventListener('click', (event) =>
    {
        carousel.style.display = 'none';
        synopsis.style.display = 'none';
        trailer.style.display = 'flex';

        buttons.forEach((button) =>
        {
            addButtonActivity(button, event)
        });
    });

    document.querySelector('#synopsis-button').addEventListener('click', (event) =>
    {
        carousel.style.display = 'none';
        trailer.style.display = 'none';
        synopsis.style.display = 'flex';

        buttons.forEach((button) =>
        {
            addButtonActivity(button, event)
        });
    });
});

// Adds a class to the button that was clicked and removes that class from any other button
function addButtonActivity(button, event)
{
    if (button === event.target)
    {
        button.classList.add('active-button');
    }
    else
    {
        button.classList.remove('active-button');
    }
}
