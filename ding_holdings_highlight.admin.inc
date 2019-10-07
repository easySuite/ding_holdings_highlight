<?php

/**
 * @file
 */

/**
 * Ding Holdings Highlight settings form.
 */
function ding_holdings_highlight_settings_form($form, &$form_state) {
  $rows_new = isset($form_state['placement_rows']) ? $form_state['placement_rows'] : 1;
  $form_state['placement_rows'] = $rows_new;

  $form['highlights'] = [
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Highlight specific holding placements') . '</h2>',
  ];

  $placements = variable_get('placements_wrapper', []);

  $form['placements_wrapper'] = [
    '#type' => 'container',
    '#tree' => TRUE,
    '#theme' => 'ding_holdings_highlight_element',
    '#prefix' => '<div id="ding-holdings-highlight-wrapper">',
    '#suffix' => '</div>',
  ];

  if (isset($form_state['input']['placements_wrapper'])) {
    $form_state['input']['placements_wrapper'] = array_values($form_state['input']['placements_wrapper']);
  }

  $rows = isset($placements) ? $placements : [];
  for ($i = 0; $i < count($rows); $i++) {
    $form['placements_wrapper'][] = ding_holdings_highlight_row_form($form_state, $rows[$i]);
  }

  for ($i = 0; $i < $rows_new; $i++) {
    $form['placements_wrapper'][] = ding_holdings_highlight_row_form($form_state);
  }

  $form['new'] = [
    '#type' => 'submit',
    '#value' => t('Add option'),
    '#ajax' => [
      'callback' => '_ding_holdings_highlight_row_callback',
      'effect' => 'fade',
      'method' => 'replace',
      'wrapper' => 'ding-holdings-highlight-wrapper',
    ],
    '#submit' => ['ding_holdings_highlight_row_submit'],
    '#limit_validation_errors' => [],
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  ];

  $form['#prefix'] = '<div id="ding-holdings-highlight-settings-form">';
  $form['#suffix'] = '</div>';

  return $form;
}

/**
 * Custom submit handler for placements form.
 *
 * @see ding_holdings_highlight_settings_form()
 */
function ding_holdings_highlight_settings_form_submit($form, &$form_state) {
  $input = $form_state['input'];
  $placements = array_filter($input['placements_wrapper'], function ($v) {
    return (!empty($v['placement']));
  });

  $placements = array_values($placements);

  variable_set('placements_wrapper', $placements);
  drupal_set_message(t('Placements saved.'));
}

/**
 * Define single row form elements.
 *
 * @param array $form_state
 *   The form state.
 * @param array $default
 *   Default form elements values, if any.
 *
 * @return array
 *   Form structure.
 */
function ding_holdings_highlight_row_form(&$form_state, $default = []) {
  $form = [];

  $form['branch'] = [
    '#type' => 'select',
    '#options' => fbs_branches(),
    '#default_value' => isset($default['branch']) ? $default['branch'] : [],
    '#empty_option' => t('Choose branch'),
  ];

  $form['placement'] = [
    '#type' => 'textfield',
    '#default_value' => isset($default['placement']) ? $default['placement'] : '',
  ];

  $form['message'] = [
    '#type' => 'textfield',
    '#default_value' => isset($default['message']) ? $default['message'] : '',
  ];

  return $form;
}

/**
 * AJAX callback for new row button.
 *
 * @see ding_holdings_highlight_settings_form()
 */
function _ding_holdings_highlight_row_callback($form, &$form_state) {
  return $form['placements_wrapper'];
}

/**
 * Custom submit handler for new row button.
 *
 * @see ding_holdings_highlight_settings_form()
 */
function ding_holdings_highlight_row_submit($form, &$form_state) {
  $form_state['placement_rows']++;
  $form_state['rebuild'] = TRUE;
}
